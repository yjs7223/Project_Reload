//// Copyright, (c) Sami Kangasmaa 2022


#include "CoverSystem.h"
#include "CoverSystemProxy.h"
#include "CoverMemory.h"
#include "CoverSolver.h"

#include "EngineUtils.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

#include "Deams51Algorithms.h"

namespace GCoverSystem
{
	static TMap<UWorld*, TWeakObjectPtr<ACoverSystem>> GCoverSystems;
}

ACoverSystem::ACoverSystem()
{
	PrimaryActorTick.bCanEverTick = true;
	Proxy = nullptr;
}

void ACoverSystem::BeginPlay()
{
	Super::BeginPlay();

	ensure(!GCoverSystem::GCoverSystems.Contains(GetWorld()) && "Multiple cover systems detected! Only single system should exists at time!");
	GCoverSystem::GCoverSystems.Add(GetWorld(), this);

	// Create proxy object
	ensure(Proxy == nullptr && "Cover system proxy wasn't null. Possible memory leak happening!");
	if (!Proxy)
	{
		Proxy = new FCoverSystemProxy();
		Proxy->Initialize();
	}

	if (bGenerateOnBeginPlay)
	{
		GenerateCovers(true, true);
	}

	// Bind to navigation generation
	if (UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		NavSystem->OnNavigationGenerationFinishedDelegate.AddDynamic(this, &ACoverSystem::OnNavigationGenerationFinished);
	}

	OccupiedCovers.Empty();

#if WITH_EDITOR
	// Debug cleanup of preview
	if (bFlushPreviewDebug)
	{
		FlushPersistentDebugLines(GetWorld());
	}
#endif
}

void ACoverSystem::EndPlay(EEndPlayReason::Type Reason)
{
	// Unbind navigation delegate	
	if (UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		NavSystem->OnNavigationGenerationFinishedDelegate.RemoveDynamic(this, &ACoverSystem::OnNavigationGenerationFinished);
	}

	// Delete proxy object. Syncs any left tasks here
	if (Proxy)
	{
		Proxy->Release();
		delete Proxy;
		Proxy = nullptr;
	}

	if (ensure(GCoverSystem::GCoverSystems.Contains(GetWorld())))
	{
		if (ensure(GCoverSystem::GCoverSystems[GetWorld()] == this))
		{
			GCoverSystem::GCoverSystems.Remove(GetWorld());
		}
	}

	Super::EndPlay(Reason);
}

void ACoverSystem::BeginDestroy()
{
	ensure(Proxy == nullptr && "Cover system proxy wasn't null. Possible memory leak happening!");
	Super::BeginDestroy();
}

void ACoverSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Tick proxy per frame from game thread to handle task lifetime
	if (Proxy)
	{
		Proxy->ObserveTask();
	}

	// Generate again, if the state is dirty
	if (bDirtyGeneration)
	{
		GenerateCovers(true, true, false);
	}

	if (bDebugDraw)
	{
		DebugDraw(GetWorld(), Proxy);
	}
}

void ACoverSystem::OnNavigationGenerationFinished(class ANavigationData* NavData)
{
	if (bGenerateOnNavigationRebuild)
	{
		bDirtyGeneration = true;
	}
}

ACoverSystem* ACoverSystem::GetCoverSystem(const UObject* WorldContext)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (World)
	{
		// First find from register. This is available only runtime for faster access
		TWeakObjectPtr<ACoverSystem>* CoverSystem = GCoverSystem::GCoverSystems.Find(World);
		if (CoverSystem && CoverSystem->IsValid())
		{
			return CoverSystem->Get();
		}

		// If not in register search from the level (slower)
		TActorIterator<ACoverSystem> Itr(World);
		if (Itr)
		{
			return *Itr;
		}
	}
	return nullptr;
}

void ACoverSystem::GenerateCovers(bool bForceRegenerate, bool bAsync, bool bDeferGenerationIfBusy)
{
	if (!Proxy)
	{
		return;
	}

#if WITH_EDITOR
	// Debug cleanup of preview
	if (bFlushPreviewDebug)
	{
		FlushPersistentDebugLines(GetWorld());
	}
#endif

	// Set trace channel
	FCoverSolver::SetCoverTraceChannel(TraceChannel.GetValue());

	// Set static sovler parameters
	FCoverSolver::SetCoverSolveParameters(BuildParams.AgentMaxWidth, BuildParams.AgentMaxHeightStanding, BuildParams.AgentMaxHeightCrouch);

	const bool bAsyncTask = bEnableAsyncMode ? bAsync : false;
	if (Proxy->GenerateCovers(bAsyncTask, bForceRegenerate, GetWorld(), BuildParams, CalculateOctreeOrigin()))
	{
		bDirtyGeneration = false; // Build was started, so generation is no more dirty
	}
	else // Build was not started
	{
		// Defer generation on failed attempt and try again later
		if (bDeferGenerationIfBusy)
		{
			bDirtyGeneration = true;
		}
	}
}

void ACoverSystem::RebaseOrigin(const FVector& NewOrigin, bool bRegenerate)
{
	SetActorLocation(NewOrigin);
	if (bRegenerate)
	{
		GenerateCovers(true, true, true);
	}
}

bool ACoverSystem::GetCoverData(const FCoverHandle& CoverHandle, FCoverData& OutData) const
{
	if (!Proxy)
	{
		return false;
	}

	FCEntityId Id = CoverMemoryUtils::ConvertHandle(CoverHandle);
	bool bFound = false;
	FCoverData Data;

	Proxy->AccessMemory([Id, &bFound, &Data](FCoverMemory* Memory)
		{
			check(Memory);
			check(Memory->GetEntities());
			bFound = Memory->GetEntities()->IsValidId(Id);
			if (bFound)
			{
				Data = (*Memory->GetEntities())[Id].Data;
			}
		});

	OutData = Data;
	return bFound;
}

void ACoverSystem::GetCoversWithinBounds(const FBoxCenterAndExtent& InBounds, TArray<FCoverHandle>& OutCoverHandles) const
{
	if (!Proxy)
	{
		return;
	}

	TArray<FCEntityId> EntityIds;
	Proxy->AccessMemory([=, &EntityIds](FCoverMemory* Memory)
		{
			check(Memory);
			FCoverSolver::GetCoversWithinBounds(InBounds, Memory, EntityIds);
		});

	OutCoverHandles.Reserve(EntityIds.Num());
	for (FCEntityId Id : EntityIds)
	{
		OutCoverHandles.Add(CoverMemoryUtils::ConvertEntityId(Id));
	}
}

void ACoverSystem::GetCoversAndDataWithinBounds(const FBoxCenterAndExtent& InBounds, TArray<FCover>& OutCovers) const
{
	if (!Proxy)
	{
		return;
	}

	TArray<FCover> Covers;
	Proxy->AccessMemory([=, &Covers](FCoverMemory* Memory)
		{
			check(Memory);
			check(Memory->GetEntities());

			TArray<FCEntityId> EntityIds;
			FCoverSolver::GetCoversWithinBounds(InBounds, Memory, EntityIds);

			Covers.Reserve(EntityIds.Num());
			for (FCEntityId EntityId : EntityIds)
			{
				const FCoverEntity& Entity = (*Memory->GetEntities())[EntityId];
				Covers.Add(FCover(CoverMemoryUtils::ConvertEntityId(EntityId), Entity.Data));
			}
		});
	OutCovers = Covers;
}

void ACoverSystem::GetCoversWithinBounds(const FBoxSphereBounds& InBounds, TArray<FCoverHandle>& OutCoverHandles) const
{
	GetCoversWithinBounds(FBoxCenterAndExtent(InBounds.GetBox()), OutCoverHandles);
}

void ACoverSystem::GetCoverDataWithinBounds(const FBoxSphereBounds& InBounds, TArray<FCover>& OutCovers) const
{
	GetCoversAndDataWithinBounds(FBoxCenterAndExtent(InBounds.GetBox()), OutCovers);
}

bool ACoverSystem::OccupyCover(AController* Controller, const FCoverHandle& CoverHandle)
{
	if (!Controller)
	{
		return false;
	}

	// Check current occupier
	AController* Occupier = GetOccupyingController(CoverHandle);
	if (Occupier)
	{
		// Already occupied by someone else
		if (Controller != Occupier)
		{
			return false;
		}
		else // Occupy success because the controller was already occupying the cover
		{
			return true;
		}
	}

	OccupiedCovers.Add(CoverHandle, Controller);
	return true;
}

void ACoverSystem::UnOccupyCover(const FCoverHandle& CoverHandle)
{
	OccupiedCovers.Remove(CoverHandle);
}

void ACoverSystem::UnOccupyCoverFromController(const FCoverHandle& CoverHandle, AController* Controller)
{
	if (!Controller)
	{
		return;
	}

	AController* Occupier = GetOccupyingController(CoverHandle);
	if (Occupier == Controller)
	{
		UnOccupyCover(CoverHandle);
	}
}

AController* ACoverSystem::GetOccupyingController(const FCoverHandle& CoverHandle) const
{
	const TWeakObjectPtr<AController>* ControllerPtr = OccupiedCovers.Find(CoverHandle);
	if (ControllerPtr && (*ControllerPtr).IsValid())
	{
		return (*ControllerPtr).Get();
	}
	return nullptr;
}

bool ACoverSystem::IsCoverOccupied(const FCoverHandle& CoverHandle) const
{
	return GetOccupyingController(CoverHandle) != nullptr;
}

bool ACoverSystem::IsAsyncWorkerRunning() const
{
	if (Proxy)
	{
		return Proxy->IsWorkerTaskRunning();
	}
	return false;
}

FVector ACoverSystem::CalculateOctreeOrigin() const
{
	FVector Origin = GetActorLocation();
	Origin.X = (float)FMath::TruncToInt(Origin.X);
	Origin.Y = (float)FMath::TruncToInt(Origin.Y);
	Origin.Z = (float)FMath::TruncToInt(Origin.Z);
	return Origin;
}

void ACoverSystem::DebugDraw(UWorld* World,FCoverSystemProxy* InProxy, bool bPersistent)
{
#if WITH_EDITOR

	if (!World)
	{
		return;
	}
	if (!InProxy)
	{
		return;
	}

	if (bPersistent)
	{
		bFlushPreviewDebug = true;
	}

	InProxy->AccessMemory([=](FCoverMemory* Memory)
		{
			const float DebugExtent = DebugDrawDistance;

			TArray<FCEntityId> EntityIds;
			FCoverSolver::GetCoversWithinBounds(FBoxCenterAndExtent(GetActorLocation(), FVector(DebugExtent)), Memory, EntityIds);

			for (const FCEntityId& Id : EntityIds)
			{
				if (!Memory->GetEntities()->IsValidId(Id))
				{
					continue;
				}

				const FCoverEntity& Entity = (*Memory->GetEntities())[Id];
				const bool bOccupied = IsCoverOccupied(CoverMemoryUtils::ConvertEntityId(Id));
				Deams51::FDeams51CoverGenerator::DebugDrawCover(World, Entity.Data, DebugExtent, BuildParams.AgentMaxHeightCrouch, BuildParams.AgentMaxHeightStanding, bOccupied, bPersistent);
			}

			if (bDrawOctreeBounds)
			{
				const FBoxCenterAndExtent OctreeBounds = Memory->GetOctree()->GetRootBounds();
				DrawDebugBox(World, OctreeBounds.Center, FVector(OctreeBounds.Extent), FQuat::Identity, FColor::Emerald, bPersistent);
			}
		});
#endif
}

#if WITH_EDITOR

void ACoverSystem::PreviewGenerate()
{
	if (!ensure(Proxy == nullptr))
	{
		return;
	}

	Proxy = new FCoverSystemProxy();
	Proxy->Initialize();

	GenerateCovers(true, false);
	DebugDraw(GetWorld(), Proxy, true);

	Proxy->Release();
	delete Proxy;
	Proxy = nullptr;
}

void ACoverSystem::CleanPreview()
{
	FlushPersistentDebugLines(GetWorld());
}

#endif