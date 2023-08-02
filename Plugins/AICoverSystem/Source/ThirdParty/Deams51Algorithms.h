#pragma once

//// Copyright (c) 2016 Deams51, Copyright (c) 2022 Sami Kangasmaa

/*
*
*** Contains modified subtantial algorithms or/and parts of code from original work of Deams51 licensed under MIT License: ***

	Copyright (c) 2016 Deams51

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

*/


#include "CoverMemory.h"
#include "CoverSystemPublicData.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

namespace Deams51
{
	class FDeams51CoverGenerator
	{
	private:

		static bool CoverExistsInOctree(const FBoxCenterAndExtent& InBounds, FCoverMemory* Memory)
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

	public:

		static void Build_AnalyzeMeshData(FCoverMemory* Memory, UWorld* World, const TArray<FVector>& NavMeshEdgeVerts, const FCoverBuildParams& Params, ECollisionChannel TraceChannel)
		{
			for (int32 Idx = 0; Idx < NavMeshEdgeVerts.Num(); Idx += 2)
			{
				if (!NavMeshEdgeVerts.IsValidIndex(Idx) || !NavMeshEdgeVerts.IsValidIndex(Idx + 1))
				{
					continue;
				}

				const FNavLocation SegmentStart = FNavLocation(NavMeshEdgeVerts[Idx]);
				const FNavLocation SegmentEnd = FNavLocation(NavMeshEdgeVerts[Idx + 1]);

				const FVector Segment = SegmentEnd.Location - SegmentStart.Location;
				const float SegmentSize = Segment.Size();
				FVector Perp = FDeams51CoverGenerator::GetPerpVector(Segment);
				FVector SegmentDirection = Segment;
				SegmentDirection.Normalize();

				// Check start and end position 			
				FDeams51CoverGenerator::Build_TestAndAddCover(SegmentStart.Location, SegmentDirection, World, Perp, Memory, Params, TraceChannel);
				FDeams51CoverGenerator::Build_TestAndAddCover(SegmentEnd.Location, SegmentDirection, World, Perp, Memory, Params, TraceChannel);

				// Check inner points if segment is long enough
				if (SegmentSize > Params.SegmentLength)
				{
					const int32 NumSegmentPieces = Segment.Size() / Params.SegmentLength;
					for (int32 idx = 1; idx < NumSegmentPieces; idx++)
					{
						const FVector SegmentPoint = SegmentStart.Location + (idx * Params.SegmentLength * SegmentDirection);
						FDeams51CoverGenerator::Build_TestAndAddCover(SegmentPoint, SegmentDirection, World, Perp, Memory, Params, TraceChannel);
					}
				}
			}
		}

		static FVector GetPerpVector(const FVector& Vector)
		{
			return FVector(Vector.Y, -Vector.X, 0.0f);
		}

		static FVector GetPerpVectorClockwise(const FVector& Vector)
		{
			return FVector(-Vector.Y, Vector.X, 0.0f);
		}

		static bool Build_IsProvidingCover(UWorld* World, const FVector& SegmentPoint, const FVector& SegmentDirection, const FVector& TraceVec, float StartHeight, float EndHeight, const FCoverBuildParams& Params, ECollisionChannel TraceChannel)
		{
			bool bHit = false;
			FCollisionQueryParams CollisionParams;
			const float HalfWidth = Params.AgentMaxWidth / 2.f;

			// First trace to find normal to geometry
			FHitResult HitResult;
			bHit = World->LineTraceSingleByChannel(HitResult, SegmentPoint, SegmentPoint + TraceVec, TraceChannel, CollisionParams);
			if (!bHit)
			{
				return false;
			}

			// Multi trace to ensure cover is safe using the normal to impact from previous trace as direction 
			FVector TraceDirection = TraceVec;
			TraceDirection.Normalize();
			TraceDirection = Params.TraceLength * TraceDirection;

			for (float xOffset = -HalfWidth; xOffset <= HalfWidth; xOffset += Params.TraceStepDistance)
			{
				for (float zOffset = StartHeight; zOffset <= EndHeight; zOffset += Params.TraceStepDistance)
				{
					const FVector StartPos = SegmentPoint + FVector(0, 0, zOffset) + xOffset * SegmentDirection;

					bHit = World->LineTraceTestByChannel(StartPos, StartPos + TraceDirection, TraceChannel, CollisionParams);
					if (!bHit)
					{
						return false;
					}
				}
			}

			return true;
		}

		static bool Build_TwoPassesCheck(UWorld* World, const FVector& SegmentPoint, const FVector& FirstTestDirection, float VerticalOffset, const FVector& CoverDirection, const FCoverBuildParams& Params, ECollisionChannel TraceChannel)
		{
			const FVector PointOff = SegmentPoint + FVector(0.f, 0.f, VerticalOffset);
			const FVector FirstTestEndPosition = PointOff + FirstTestDirection;
			FQuat Quat = FVector::UpVector.ToOrientationRotator().Quaternion();
			Quat.Normalize();

			FHitResult HitResult;

			//Check if it is a left cover, a right cover, or a crouch cover
			FCollisionQueryParams CollisionParams;

			bool HitOnSide = World->SweepSingleByChannel(HitResult, PointOff, FirstTestEndPosition, Quat, TraceChannel, FCollisionShape::MakeSphere(Params.SweepTraceRadius), CollisionParams);

			if (!HitOnSide)
			{
				FHitResult FrontResult, DownResult;

				// Side Front 
				const FVector SideFrontStart = FirstTestEndPosition;
				const FVector SideFrontEnd = SideFrontStart + Params.OffsetFrontAim * CoverDirection;
				bool HitSideFront = World->SweepSingleByChannel(FrontResult, SideFrontStart, SideFrontEnd, Quat, TraceChannel, FCollisionShape::MakeSphere(Params.SweepTraceRadius), CollisionParams);

				// Side Bottom = Floor check
				const FVector SideDownStart = PointOff + FVector(FirstTestDirection.X, FirstTestDirection.Y, 0.f);
				const FVector SideDownEnd = SideDownStart + FVector::UpVector * (-100);
				bool HitSideBottom = World->SweepSingleByChannel(DownResult, SideDownStart, SideDownEnd, Quat, TraceChannel, FCollisionShape::MakeSphere(Params.SweepTraceRadius), CollisionParams);

				// Should not hit on side front to allow aiming, but should hit on side bottom to avoid going through the floor
				if (!HitSideFront && HitSideBottom)
					return true;
			}
			return false;
		}

		static bool Build_SolveCover(UWorld* World, const FVector& SegmentPoint, const FVector& Segment, const FVector& TraceVec, const FVector& Perp, const FCoverBuildParams& Params, FCoverData& OutCoverData, ECollisionChannel TraceChannel)
		{
			FVector CoverDirection = -Perp;
			CoverDirection.Normalize();

			// Check if this position hides a crouched character when seen from the perpendicular direction 
			// That's the minimum requirement for being a cover 
			bool ProvidesCoverCrouched = Build_IsProvidingCover(World, SegmentPoint, Segment, TraceVec, 0, Params.AgentMaxHeightCrouch, Params, TraceChannel);
			if (ProvidesCoverCrouched)
			{
				const FVector LeaningRightVec = Params.OffsetWhenLeaningSides * (Segment);
				const FVector LeaningLeftVec = -LeaningRightVec;

				// Check if it is a right or left or front cover for crouched position 
				const float HeightCrouchingCheck = Params.AgentMaxHeightCrouch / 2.f;
				bool CanShootFromLeftCrouched = Build_TwoPassesCheck(World, SegmentPoint, LeaningLeftVec, HeightCrouchingCheck, CoverDirection, Params, TraceChannel);
				bool CanShootFromRightCrouched = Build_TwoPassesCheck(World, SegmentPoint, LeaningRightVec, HeightCrouchingCheck, CoverDirection, Params, TraceChannel);
				bool CanShootFromFrontCrouched = Build_TwoPassesCheck(World, SegmentPoint, Params.AgentMaxHeightStanding * FVector::UpVector - 30.f, HeightCrouchingCheck, CoverDirection, Params, TraceChannel); // -30.f for the offset between the point and the floor

				// Check for standing (No front in that case) 
				bool CanShootFromLeftStanding = false;
				bool CanShootFromRightStanding = false;
				bool ProvidesCoverStanding = Build_IsProvidingCover(World, SegmentPoint, Segment, TraceVec, Params.AgentMaxHeightCrouch + Params.TraceStepDistance, Params.AgentMaxHeightStanding, Params, TraceChannel);
				if (ProvidesCoverStanding)
				{
					const float HeightStandingCheck = Params.AgentMaxHeightCrouch + ((Params.AgentMaxHeightStanding - Params.AgentMaxHeightCrouch) / 2.f);
					CanShootFromLeftStanding = CanShootFromLeftCrouched;
					CanShootFromRightStanding = CanShootFromRightCrouched;
				}

				if (CanShootFromLeftCrouched || CanShootFromRightCrouched || CanShootFromFrontCrouched || CanShootFromLeftStanding || CanShootFromRightStanding)
				{
					OutCoverData.Location = SegmentPoint;
					OutCoverData.SetDirectionToWall(CoverDirection);

					OutCoverData.bLeftCoverCrouched = CanShootFromLeftCrouched;
					OutCoverData.bRightCoverCrouched = CanShootFromRightCrouched;
					OutCoverData.bFrontCoverCrouched = CanShootFromFrontCrouched;

					OutCoverData.bLeftCoverStanding = CanShootFromLeftStanding;
					OutCoverData.bRightCoverStanding = CanShootFromRightStanding;

					OutCoverData.bCrouchedCover = !ProvidesCoverStanding;

					return true;
				}
			}

			return false;
		}

		static void Build_TestAndAddCover(const FVector& SegmentPoint, const FVector& SegmentDirection, UWorld* World, const FVector& Perp, FCoverMemory* Memory, const FCoverBuildParams& Params, ECollisionChannel TraceChannel)
		{
			check(Memory);
			check(World);
			check(Memory->GetEntities());
			check(Memory->GetOctree());

			// Check that there's no point at a distance of less than MinSpaceBetweenValidPoints already added
			const bool bAlreadyCoverWithinbounds = CoverExistsInOctree(FBoxCenterAndExtent(SegmentPoint, FVector(Params.MinSpaceBetweenValidPoints)), Memory);
			if (bAlreadyCoverWithinbounds)
				return;

			FVector TraceDirection = GetPerpVector(SegmentDirection);
			TraceDirection.Normalize();
			const FVector TraceVec = Params.TraceLength * TraceDirection;

			FCoverData CoverData;
			bool bValidCover = false;

			// Check if valid point in first direction 
			bValidCover = Build_SolveCover(World, SegmentPoint, -SegmentDirection, -TraceVec, Perp, Params, CoverData, TraceChannel);

			// If not then let's try the other side
			if (!bValidCover)
			{
				bValidCover = Build_SolveCover(World, SegmentPoint, SegmentDirection, TraceVec, Perp, Params, CoverData, TraceChannel);
			}

			// If found a valid cover, add to storage as entity and store handle to it into the octree
			if (bValidCover)
			{
				FCoverEntity Entity;
				Entity.Data = CoverData;

				// Insert into entity storage
				const FCEntityId EntityId = Memory->GetEntities()->Insert(MoveTemp(Entity));
				FCoverEntity& EntityRef = (*Memory->GetEntities())[EntityId];
				EntityRef.EntityId = EntityId;

				// Insert into octree (only entity id is stored into the tree)
				FCoverOctreeElement CoverOctreeElement(EntityRef);
				Memory->GetOctree()->AddElement(CoverOctreeElement);
			}
		}

		static void DebugDrawCoverDirections(UWorld* World, const FVector& Origin, const FVector& Direction, const FVector& Forward, float LineLength, bool bPersistent = false)
		{
			const FVector End = Origin + LineLength * Direction;
			DrawDebugLine(World, Origin, End, FColor::Cyan, bPersistent);
			DrawDebugDirectionalArrow(World, End, End + LineLength * Forward, 15.0f, FColor::Cyan, bPersistent);
		}

		static void DebugDrawCover(UWorld* World, const FCoverData& Data, float DebugExtent, float CrouchHeight, float StandHeight, bool bOccupied, bool bPersistent)
		{
			const FVector CrouchOffset = FVector(0.f, 0.f, CrouchHeight);
			const FVector StandingOffset = FVector(0.f, 0.f, StandHeight);

			FVector Perp = Deams51::FDeams51CoverGenerator::GetPerpVector(Data.GetDirectionToWall());
			Perp.Normalize();

			const FVector Left = Perp;
			const FVector Right = -Perp;
			const FVector Front = Data.GetDirectionToWall();
			const FVector Top = FVector::UpVector;

			if (Data.bLeftCoverCrouched || Data.bRightCoverCrouched || Data.bFrontCoverCrouched)
			{
				const FVector CrouchLocation = Data.Location + CrouchOffset;

				const FColor DBColor = bOccupied ? FColor::Red : FColor::Blue;
				DrawDebugSphere(World, CrouchLocation, 32.0f, 4, DBColor, bPersistent);

				if (Data.bLeftCoverCrouched)
					DebugDrawCoverDirections(World, CrouchLocation, Left, Front, 60.0f, bPersistent);
				if (Data.bRightCoverCrouched)
					DebugDrawCoverDirections(World, CrouchLocation, Right, Front, 60.0f, bPersistent);
				if (Data.bFrontCoverCrouched)
					DebugDrawCoverDirections(World, CrouchLocation, Top, Front, 60.0f, bPersistent);

			}

			if (Data.bLeftCoverStanding || Data.bRightCoverStanding)
			{
				const FVector StandingLocation = Data.Location + StandingOffset;

				const FColor DBColor = bOccupied ? FColor::Red : FColor::Yellow;
				DrawDebugSphere(World, StandingLocation, 32.0f, 4, DBColor, bPersistent);

				if (Data.bLeftCoverStanding)
					DebugDrawCoverDirections(World, StandingLocation, Left, Front, 60.0f, bPersistent);
				if (Data.bRightCoverStanding)
					DebugDrawCoverDirections(World, StandingLocation, Right, Front, 60.0f, bPersistent);
			}
		}
	};
}