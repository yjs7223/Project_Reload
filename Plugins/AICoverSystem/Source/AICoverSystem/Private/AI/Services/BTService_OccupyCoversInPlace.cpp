// Copyright (c), Sami Kangasmaa 2022


#include "AI/Services/BTService_OccupyCoversInPlace.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AI/BlackboardKeyType_Cover.h"
#include "VisualLogger/VisualLogger.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

UBTService_OccupyCoversInPlace::UBTService_OccupyCoversInPlace()
{
	NodeName = "Occupy overlapping covers";

	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
	bTickIntervals = true;

	Interval = 0.25f;
	RandomDeviation = 0.0f;
}

void UBTService_OccupyCoversInPlace::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);

	FOccupyCoverInPlaceServiceMemory* MyMemory = CastInstanceNodeMemory<FOccupyCoverInPlaceServiceMemory>(NodeMemory);
	*MyMemory = FOccupyCoverInPlaceServiceMemory();
}

void UBTService_OccupyCoversInPlace::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	FOccupyCoverInPlaceServiceMemory* MyMemory = CastInstanceNodeMemory<FOccupyCoverInPlaceServiceMemory>(NodeMemory);
	check(MyMemory);

	AAIController* OwnerController = OwnerComp.GetAIOwner();
	if (OwnerController != nullptr)
	{
		OccupyCoversInBounds(OwnerController, MyMemory);
	}
}

void UBTService_OccupyCoversInPlace::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	FOccupyCoverInPlaceServiceMemory* MyMemory = CastInstanceNodeMemory<FOccupyCoverInPlaceServiceMemory>(NodeMemory);
	check(MyMemory);

	AAIController* OwnerController = OwnerComp.GetAIOwner();
	if (OwnerController != nullptr)
	{
		ReleaseOccupiedCovers(OwnerController, MyMemory);
	}
}

void UBTService_OccupyCoversInPlace::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	FOccupyCoverInPlaceServiceMemory* MyMemory = CastInstanceNodeMemory<FOccupyCoverInPlaceServiceMemory>(NodeMemory);
	check(MyMemory);

	AAIController* OwnerController = OwnerComp.GetAIOwner();
	if (OwnerController)
	{
		OccupyCoversInBounds(OwnerController, MyMemory);
	}
}

void UBTService_OccupyCoversInPlace::OccupyCoversInBounds(AAIController* Controller, FOccupyCoverInPlaceServiceMemory* Memory)
{
	check(Memory);
	check(Controller);

	ACoverSystem* CoverSystem = ACoverSystem::GetCoverSystem(Controller);
	if (!CoverSystem)
	{
		return;
	}

	TArray<FCoverHandle> CoversToOccupy;
	QueryCoverHandles(Controller, CoversToOccupy);

	// Release all covers that are not in bounds anymore
	ReleaseOccupiedCovers(Controller, Memory, &CoversToOccupy);

	// Occupy all that were queried
	for (const FCoverHandle& Handle : CoversToOccupy)
	{
		if (CoverSystem->OccupyCover(Controller, Handle))
		{
			Memory->OccupiedHandles.Add(Handle);
		}
	}
}

void UBTService_OccupyCoversInPlace::QueryCoverHandles(AAIController* Controller, TArray<FCoverHandle>& Handles) const
{
	check(Controller);

	APawn* Pawn = Controller->GetPawn();
	if (!Pawn)
	{
		return;
	}

	// Solve bounds to use
	FVector Extents = BoundExtents;
	FVector Origin = Pawn->GetActorLocation();
	UCapsuleComponent* Capsule = nullptr;
	if (ACharacter* Character = Cast<ACharacter>(Pawn))
	{
		Capsule = Character->GetCapsuleComponent();
		if (Capsule)
		{
			Extents = Capsule->Bounds.BoxExtent;
		}
	}

	// Find covers inside bounds
	ACoverSystem* CoverSystem = ACoverSystem::GetCoverSystem(Controller);
	if (!CoverSystem)
	{
		return;
	}

	CoverSystem->GetCoversWithinBounds(FBoxCenterAndExtent(Origin, Extents), Handles);
}

void UBTService_OccupyCoversInPlace::ReleaseOccupiedCovers(AAIController* Controller, FOccupyCoverInPlaceServiceMemory* Memory, TArray<FCoverHandle>* IgnoreHandles)
{
	check(Memory);
	check(Controller);

	ACoverSystem* CoverSystem = ACoverSystem::GetCoverSystem(Controller);
	if (!CoverSystem)
	{
		return;
	}

	TArray<FCoverHandle, TInlineAllocator<8>> RemoveList;
	for (const FCoverHandle& Handle : Memory->OccupiedHandles)
	{
		if (IgnoreHandles && IgnoreHandles->Contains(Handle))
		{
			continue;
		}
		RemoveList.Add(Handle);
	}

	for (const FCoverHandle& Handle : RemoveList)
	{
		CoverSystem->UnOccupyCoverFromController(Handle, Controller);
		Memory->OccupiedHandles.Remove(Handle);
	}
}