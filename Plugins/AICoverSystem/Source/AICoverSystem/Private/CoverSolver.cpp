//// Copyright, (c) Sami Kangasmaa 2022


#include "CoverSolver.h"
#include "CoverMemory.h"
#include "Deams51Algorithms.h"

namespace CoverSolver
{
	static ECollisionChannel SolverTraceChannel = ECC_WorldStatic;
	static float AgentWidth = 80.0f;
	static float AgentHeightStand = 180.0f;
	static float AgentHeightCrouch = 80.0f;
}

void FCoverSolver::SetCoverTraceChannel(ECollisionChannel Channel)
{
	CoverSolver::SolverTraceChannel = Channel;
}

ECollisionChannel FCoverSolver::GetCoverTraceChannel()
{
	return CoverSolver::SolverTraceChannel;
}

void FCoverSolver::SetCoverSolveParameters(float InAgentWidth, float InAgentHeightStand, float InAgentHeightCrouch)
{
	CoverSolver::AgentWidth = InAgentWidth;
	CoverSolver::AgentHeightStand = InAgentHeightStand;
	CoverSolver::AgentHeightCrouch = InAgentHeightCrouch;
}

void FCoverSolver::GetOctreeElementsWithinBounds(const FBoxCenterAndExtent& BoundsIn, FCoverMemory* Memory, TArray<FCoverOctreeElement>& OutElements)
{
	check(Memory);
	check(Memory->GetOctree());

	Memory->GetOctree()->FindElementsWithBoundsTest<>(BoundsIn, [&OutElements](const FCoverOctreeElement& Element)
		{
			OutElements.Add(Element);
		});
}

void FCoverSolver::GetCoversWithinBounds(const FBoxCenterAndExtent& BoundsIn, FCoverMemory* Memory, TArray<FCEntityId>& OutEntityIds)
{
	check(Memory);
	check(Memory->GetOctree());

	Memory->GetOctree()->FindElementsWithBoundsTest<>(BoundsIn, [&OutEntityIds](const FCoverOctreeElement& Element)
		{
			OutEntityIds.Add(Element.EntityId);
		});
}

bool FCoverSolver::CoverExistWithinBounds(const FBoxCenterAndExtent& InBounds, FCoverMemory* Memory)
{
	check(Memory);
	check(Memory->GetOctree());

	bool bHasElement = false;
	Memory->GetOctree()->FindFirstElementWithBoundsTest<>(InBounds, [&bHasElement](const FCoverOctreeElement& Element)
		{
			bHasElement = true;
			return false;
		});

	return bHasElement;
}

bool FCoverSolver::TraceToCover(UWorld* World, const FCover& Cover, const FVector& From, ECollisionChannel TraceChannel, const TArray<AActor*>* IgnoredActors)
{
	if (!ensure(World))
	{
		return false;
	}

	FCollisionQueryParams QueryParams = FCollisionQueryParams(TEXT("CoverTrace"), true);
	if (IgnoredActors)
	{
		QueryParams.AddIgnoredActors(*IgnoredActors);
	}

	const FVector Location = Cover.Data.Location;
	const FVector Forward = Cover.Data.GetDirectionToWall();
	const FVector Right = Deams51::FDeams51CoverGenerator::GetPerpVectorClockwise(Forward);

	const float AgentHalfWidth = CoverSolver::AgentWidth * 0.5f;

	const bool bLeft = World->LineTraceTestByChannel(From, Location + AgentHalfWidth * (-Right), TraceChannel, QueryParams);
	const bool bRight = World->LineTraceTestByChannel(From, Location + AgentHalfWidth * Right, TraceChannel, QueryParams);

	return bLeft && bRight;
}