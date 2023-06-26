//// Copyright, (c) Sami Kangasmaa 2022

#pragma once

#include "CoreMinimal.h"
#include "CoverMemory.h"
#include "CoverGenerateAsyncTask.h"
#include "CoverSystemPublicData.h"

#include "Engine/World.h"

/**
 * Proxy class to act between game thread and async generation thread
 */
class FCoverSystemProxy
{
private:

	bool bInitialized;
	bool bGenerated;

public:

	FCoverSystemProxy();
	~FCoverSystemProxy();

	void Initialize();

	// Releases the proxy, ensures that all async tasks are completed before returning
	void Release();

	// Called per frame to cleanup work task
	void ObserveTask();

private:

	/*
	* Shared cover memory between game and worker thread
	* Memory can be locked during swap by worker task
	*/
	FCoverMemory* CoverMemory;

	// Mutex to control access into cover memory
	FCriticalSection MemoryCriticalSection;

public:

	// Safe access to memory from multiple threads via lambda.
	void AccessMemory(TFunctionRef< void(FCoverMemory*)> Func);

	/*
	* Swaps the memory accessible by gamethread to a newly built one
	* This is called from work task once the build has been completed to minimize lock time
	*/
	void SwapMemory(FCoverMemory* NewMemory);

public:

	// Generates entities of covers and stores them into a entity container
	bool GenerateCovers(bool bAsync, bool bRegenerate, UWorld* World, const FCoverBuildParams& Params, const FVector& Origin);

	// Checks if worker task is currently running
	bool IsWorkerTaskRunning() const;

protected: // Async and internal generation

	friend class FCoverGenerateAsyncTask;

	// Async task that is currently running
	TSharedPtr<FAsyncTask<FCoverGenerateAsyncTask>> RunningAsyncTask;

	// Fetches a main nav mesh from the given world
	ARecastNavMesh* GetNavMesh(UWorld* World) const;
};
