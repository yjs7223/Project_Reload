//// Copyright, (c) Sami Kangasmaa 2022

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Async/AsyncWork.h"
#include "NavMesh/RecastNavMesh.h"
#include "NavigationSystem.h"
#include "Engine/World.h"

#include "CoverSystemPublicData.h"

class FCoverSystemProxy;

/**
 * Task to generate cover data in background thread
 */
class FCoverGenerateAsyncTask : public FNonAbandonableTask 
{
public:

	friend class FAsyncTask<FCoverGenerateAsyncTask>;

	FCoverGenerateAsyncTask(UWorld* World, FCoverSystemProxy* InProxy, ARecastNavMesh* NavMesh, const FCoverBuildParams& Params, const FVector& InOrigin);
	~FCoverGenerateAsyncTask();

	FCoverSystemProxy* Proxy;
	TWeakObjectPtr<ARecastNavMesh> NavMeshPtr;
	TWeakObjectPtr<UWorld> WorldPtr;
	FCoverBuildParams BuildParams;
	FVector Origin;

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FCoverGenerateAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	void DoWork();
};
