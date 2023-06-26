//// Copyright, (c) Sami Kangasmaa 2022

/*
*
* Contains modified subtantial algorithms or/and parts of code from original work of Deams51 licensed under MIT License:

	Copyright (c) 2016 Deams51

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

* THE THIRD PARTY SNIPPETS USED IN CODE ARE MARKED WITH COMMENTED SECTIONS.

*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CoverSystemVersion.h"
#include "CoverSystemPublicData.generated.h"

/**
* Build parameters for the cover generation
*/
USTRUCT(BlueprintType)
struct AICOVERSYSTEM_API FCoverBuildParams
{
	GENERATED_BODY()

public:

	// -------- Code from Deams51 work [Section begin] ------------ //

	// Length of nav mesh sub segments
	UPROPERTY(Category = Cover, EditAnywhere, BlueprintReadWrite)
	float SegmentLength = 10.0f;

	// Length of trace when trying to find blocking geometry in direction of nav segment
	UPROPERTY(Category = Cover, EditAnywhere, BlueprintReadWrite)
	float TraceLength = 100.0f;

	// Raytrace substep distance when finding cover geometry
	UPROPERTY(Category = Cover, EditAnywhere, BlueprintReadWrite)
	float TraceStepDistance = 20.0f;

	/** Size of the sphere sweep used to determine if cover can be used to shoot to certain directions */
	UPROPERTY(Category = Cover, EditAnywhere, BlueprintReadWrite)
	float SweepTraceRadius = 5.0f;

	// Minimum space between generated covers
	UPROPERTY(Category = Cover, EditAnywhere, BlueprintReadWrite)
	float MinSpaceBetweenValidPoints = 50.0f;

	/**
	* The maximum width of the agents.
	* Generation ensures that agent with this width is fully covered behind an object if standing at cover
	*/
	UPROPERTY(Category = Cover, EditAnywhere, BlueprintReadWrite)
	float AgentMaxWidth = 80.0f;

	/** The maximum height of the agents when crouching */
	UPROPERTY(Category = Cover, EditAnywhere, BlueprintReadWrite)
	float AgentMaxHeightCrouch = 80.0f;

	/** The maximum height of the agents when standing */
	UPROPERTY(Category = Cover, EditAnywhere, BlueprintReadWrite)
	float AgentMaxHeightStanding = 180.0f;

	/** The distance between in cover position and leaning out of the cover on the sides */
	UPROPERTY(Category = Cover, EditAnywhere, BlueprintReadWrite)
	float OffsetWhenLeaningSides = 65.0f;

	/** The distance in front of a shooting position that must be free */
	UPROPERTY(Category = Cover, EditAnywhere, BlueprintReadWrite)
	float OffsetFrontAim = 100.0f;

	// -------- Code from Deams51 work [Section end] ------------ //

	/**
	* Extent of octree from the origin of cover system actor.
	* Increase this to generate covers further away.
	* In large worlds, the cover system should be repositioned and regenerated.
	* In the future, adaptive generation could be added.
	*/
	UPROPERTY(Category = Cover, EditAnywhere, BlueprintReadWrite, AdvancedDisplay, meta = (ClampMin = "1024"))
	float OctreeExtent = 64000.0f;
};

/**
 * Represents data of single cover
 */
USTRUCT(BlueprintType)
struct AICOVERSYSTEM_API FCoverData
{
	GENERATED_BODY()

public:

	// -------- Code from Deams51 work [Section begin] ------------ //

	/**  Location of the cover */
	UPROPERTY(Category = Cover, VisibleAnywhere, BlueprintReadOnly)
	FVector Location;

	/** Rotator from X of direction to wall */
	UPROPERTY(Category = Cover, VisibleAnywhere, BlueprintReadOnly)
	FRotator Rotation;

	/** Direction to wall (Perpendicular to cover) */
	UPROPERTY(Category = Cover, VisibleAnywhere, BlueprintReadOnly)
	FVector DirectionToWall;

	/** Is it a Left cover (can lean on left) */
	UPROPERTY(Category = Cover, VisibleAnywhere, BlueprintReadOnly)
	bool bLeftCoverStanding;

	/** Is it a Right cover (can lean on Right) */
	UPROPERTY(Category = Cover, VisibleAnywhere, BlueprintReadOnly)
	bool bRightCoverStanding;

	/** Is it a Left cover (can lean on left) */
	UPROPERTY(Category = Cover, VisibleAnywhere, BlueprintReadOnly)
	bool bLeftCoverCrouched;

	/** Is it a Right cover (can lean on Right) */
	UPROPERTY(Category = Cover, VisibleAnywhere, BlueprintReadOnly)
	bool bRightCoverCrouched;

	/** Is it a front cover, so crouch gives cover and while standing there's line of sight for shooting */
	UPROPERTY(Category = Cover, VisibleAnywhere, BlueprintReadOnly)
	bool bFrontCoverCrouched;

	/** Is it a cover requiring crouch */
	UPROPERTY(Category = Cover, VisibleAnywhere, BlueprintReadOnly)
	bool bCrouchedCover;

public:

	FVector GetDirectionToWall() const { return DirectionToWall; }
	void SetDirectionToWall(const FVector& Direction)
	{
		DirectionToWall = Direction;
		Rotation = DirectionToWall.ToOrientationRotator();
	}

	// -------- Code from Deams51 work [Section end] ------------ //

	FCoverData()
	{
		Location = FVector::ZeroVector;
		Rotation = FRotator::ZeroRotator;
		DirectionToWall = FVector::ZeroVector;
		bLeftCoverStanding = false;
		bRightCoverStanding = false;
		bLeftCoverCrouched = false;
		bRightCoverCrouched = false;
		bFrontCoverCrouched = false;
		bCrouchedCover = false;
	}
};



// Wrapper for cover entity access
USTRUCT(BlueprintType)
struct AICOVERSYSTEM_API FCoverHandle
{
	GENERATED_BODY()

public:

	FCoverHandle();
	FCoverHandle(uint64_t InValue);
	uint64_t Get() const { return handle; }
	bool IsValid() const;

	bool operator==(const FCoverHandle& Other) const
	{
		return handle == Other.handle;
	}

	bool operator!=(const FCoverHandle& Other) const
	{
		return handle != Other.handle;
	}

	friend uint32 GetTypeHash(const FCoverHandle& Handle)
	{
		return ::GetTypeHash(Handle.handle);
	}

private:

	uint64_t handle;
};

/**
 * Represents a single cover.
 */
USTRUCT(BlueprintType)
struct AICOVERSYSTEM_API FCover
{
	GENERATED_BODY()

public:

	FCover()
	{
		Handle = FCoverHandle();
		Data = FCoverData();
	}

	FCover(const FCoverHandle& InHandle, const FCoverData& InData)
	{
		Handle = InHandle;
		Data = InData;
	}

	// Handle to the cover.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cover")
	FCoverHandle Handle;

	// Data of the cover
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cover")
	FCoverData Data;

public:

	inline bool Equals(const FCover& Other) const 
	{
		return Handle == Other.Handle;
	}

	bool operator==(const FCover& Other) const
	{
		return Equals(Other);
	}

	bool operator!=(const FCover& Other) const
	{
		return !(Equals(Other));
	}

	inline void Invalidate()
	{
		Handle = FCoverHandle();
	}

	inline bool IsValid() const
	{
		return Handle.IsValid();
	}
};