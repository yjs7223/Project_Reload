//// Copyright, (c) Sami Kangasmaa 2022


#include "CoverGenerateAsyncTask.h"
#include "CoverSystemProxy.h"
#include "CoverBuilder.h"
#include "CoverMemory.h"


FCoverGenerateAsyncTask::FCoverGenerateAsyncTask(UWorld* World, FCoverSystemProxy* InProxy, ARecastNavMesh* NavMesh, const FCoverBuildParams& Params, const FVector& InOrigin)
{
	check(World);
	check(InProxy);
	check(NavMesh);

	Proxy = InProxy;
	WorldPtr = World;
	NavMeshPtr = NavMesh;
	BuildParams = Params;
	Origin = InOrigin;
}

FCoverGenerateAsyncTask::~FCoverGenerateAsyncTask() {}

void FCoverGenerateAsyncTask::DoWork()
{
	if (!WorldPtr.IsValid(false, true))
	{
		return;
	}
	if (!NavMeshPtr.IsValid(false, true))
	{
		return;
	}

	check(Proxy);

	/*
	* Create a new memory block to build results without locking the current memory in the proxy
	* Once the results are ready, the memory will be swapped, so game thread can access it
	*/
	FCoverMemory* Memory = new FCoverMemory(BuildParams.OctreeExtent, Origin);

	FCoverBuilder Builder;
	if (Builder.Build(Memory, WorldPtr.Get(), NavMeshPtr.Get(), BuildParams))
	{
		// Swap the memory in proxy by syncing the pointer between this and game thread
		check(Memory);
		Proxy->SwapMemory(Memory);
	}
	else // Build failed, discard the memory
	{
		delete Memory;
	}
}