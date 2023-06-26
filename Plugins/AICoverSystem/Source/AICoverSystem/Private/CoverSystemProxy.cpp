//// Copyright, (c) Sami Kangasmaa 2022


#include "CoverSystemProxy.h"
#include "CoverSolver.h"

FCoverSystemProxy::FCoverSystemProxy()
{
	CoverMemory = nullptr;
	bInitialized = false;
	bGenerated = false;
}

FCoverSystemProxy::~FCoverSystemProxy()
{
	check(!bInitialized);
}

void FCoverSystemProxy::Initialize()
{
	check(IsInGameThread());

	check(CoverMemory == nullptr);
	CoverMemory = new FCoverMemory(1024.0f, FVector::ZeroVector);
	bInitialized = true;
	bGenerated = false;
}

void FCoverSystemProxy::Release()
{
	check(IsInGameThread());

	// Ensure that async task completes before releasing the proxy
	if (RunningAsyncTask.IsValid())
	{
		RunningAsyncTask->EnsureCompletion(true);
		RunningAsyncTask.Reset();
	}

	check(CoverMemory);
	delete CoverMemory;
	CoverMemory = nullptr;
	bGenerated = false;
	bInitialized = false;
}

void FCoverSystemProxy::ObserveTask()
{
	check(IsInGameThread());

	// Check if the running async task is completed and delete it in that case
	if (RunningAsyncTask.IsValid())
	{
		if (RunningAsyncTask->IsDone())
		{
			RunningAsyncTask.Reset();
			bGenerated = true;
		}
	}
}

void FCoverSystemProxy::SwapMemory(FCoverMemory* NewMemory)
{
	// Access the memory block and swap it to the new one
	this->AccessMemory([=](FCoverMemory* Memory)
		{
			if (CoverMemory)
			{
				delete CoverMemory;
			}
			CoverMemory = NewMemory;
		});
}

void FCoverSystemProxy::AccessMemory(TFunctionRef< void(FCoverMemory*)> Func)
{
	// Lock before accessing the memory
	FScopeLock ScopeLock(&MemoryCriticalSection);

	// Call function and pass locked memory
	Func(CoverMemory);
}


bool FCoverSystemProxy::GenerateCovers(bool bAsync, bool bRegenerate, UWorld* World, const FCoverBuildParams& Params, const FVector& Origin)
{
	check(IsInGameThread());

	check(bInitialized);
	check(World);

	if (bGenerated && !bRegenerate)
	{
		return true;
	}

	// Async task already running, don't start a generation yet
	if (RunningAsyncTask.IsValid())
	{
		return false;
	}

	ARecastNavMesh* NavMesh = GetNavMesh(World);
	if (!NavMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cover generation can't be done because navigation is missing!"));
		return true;
	}

	// Run build task
	FAsyncTask<FCoverGenerateAsyncTask>* Task = new FAsyncTask<FCoverGenerateAsyncTask>(World, this, NavMesh, Params, Origin);
	check(Task);

	if (bAsync) // Async
	{
		RunningAsyncTask = MakeShareable(Task);
		Task->StartBackgroundTask();
	}
	else // Sync
	{
		Task->StartSynchronousTask();
		Task->EnsureCompletion();
		delete Task;
		bGenerated = true;
	}

	return true;
}

bool FCoverSystemProxy::IsWorkerTaskRunning() const
{
	if (RunningAsyncTask.IsValid())
	{
		return !RunningAsyncTask->IsDone();
	}
	return false;
}

ARecastNavMesh* FCoverSystemProxy::GetNavMesh(UWorld* World) const
{
	const UNavigationSystemBase* NavSystem = World->GetNavigationSystem();
	if (!NavSystem )
	{
		return nullptr;
	}
	return const_cast<ARecastNavMesh*>(static_cast<const ARecastNavMesh*>(NavSystem->GetMainNavData()));
}