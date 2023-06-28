//// Copyright, (c) Sami Kangasmaa 2022

#pragma once

#include "CoreMinimal.h"
#include "CoverSystemVersion.h"
#include "CoverMemory.h"
#include "CoverSystemPublicData.h"
#include "Engine/World.h"

/**
 * Stateless solver class to compute cover specific operations
 */
class FCoverSolver
{
private:

	FCoverSolver() {}

public:

	static void SetCoverTraceChannel(ECollisionChannel Channel);
	static ECollisionChannel GetCoverTraceChannel();
	static void SetCoverSolveParameters(float InAgentWidth, float InAgentHeightStand, float InAgentHeightCrouch);

public:

	static void GetOctreeElementsWithinBounds(const FBoxCenterAndExtent& BoundsIn, FCoverMemory* Memory, TArray<FCoverOctreeElement>& OutElements);

	static void GetCoversWithinBounds(const FBoxCenterAndExtent& BoundsIn, FCoverMemory* Memory, TArray<FCEntityId>& OutEntityIds);

	static bool CoverExistWithinBounds(const FBoxCenterAndExtent& InBounds, FCoverMemory* Memory);

	static bool TraceToCover(UWorld* World, const FCover& Cover, const FVector& From, ECollisionChannel TraceChannel, const TArray<AActor*>* IgnoredActors = nullptr);
};
