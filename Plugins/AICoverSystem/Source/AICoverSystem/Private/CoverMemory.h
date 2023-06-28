//// Copyright, (c) Sami Kangasmaa 2022

#pragma once

#include "CoreMinimal.h"
#include "CoverSystemVersion.h"
#include "CEntities.h"

#include "CoverSystemPublicData.h"
#include "Math/GenericOctree.h"
#include "NavigationSystem.h"
#include "Navmesh/RecastNavMesh.h"

/**
 * Entity that is stored into entity container per cover
 */
struct FCoverEntity
{
	// Id of this entity in the entity container
	FCEntityId EntityId;

	// Generated data of this cover.
	FCoverData Data;
};

struct FCoverOctreeElement
{
	/** Entity id of the cover */
	FCEntityId EntityId;

	FBoxSphereBounds Bounds;

	FCoverOctreeElement(const FCoverEntity& InEntity)
		: EntityId(InEntity.EntityId)
	{
		Bounds = FBoxSphereBounds(&(InEntity.Data.Location), 1);
	}
};

struct FCoverOctreeSemantics
{
	enum { MaxElementsPerLeaf = 16 };
	enum { MinInclusiveElementsPerNode = 7 };
	enum { MaxNodeDepth = 12 };

	typedef FDefaultAllocator ElementAllocator;

	FORCEINLINE static bool AreElementsEqual(const FCoverOctreeElement& A, const FCoverOctreeElement& B)
	{
		return A.EntityId == B.EntityId;
	}

	static void SetElementId(const FCoverOctreeElement& Element, FOctreeElementId2 Id) {}

	FORCEINLINE static const FBoxSphereBounds& GetBoundingBox(const FCoverOctreeElement& Element)
	{
		return Element.Bounds;
	}
};

typedef TOctree2<FCoverOctreeElement, FCoverOctreeSemantics> FCoverOctree;

/**
 * Memory structure of cover entities and octree
 */
struct FCoverMemory
{

private:

	TCEntities<FCoverEntity>* Entities;
	FCoverOctree* Octree;
	float OctreeExtent;
	FVector OctreeOrigin;

	void Initialize();
	void Release();

public:

	FCoverMemory(float InOctreeExtent, FVector InOctreeOrigin);
	~FCoverMemory();

	void Reset();

	TCEntities<FCoverEntity>* GetEntities() const 
	{ 
		check(Entities); 
		return Entities; 
	}

	FCoverOctree* GetOctree() const 
	{ 
		check(Octree); 
		return Octree;
	}

	void BuildOctree();
};

namespace CoverMemoryUtils
{
	inline FCEntityId ConvertHandle(const FCoverHandle& Handle)
	{
		FCEntityId Id = InvalidCEntityId;
		Id.value = Handle.Get();
		return Id;
	}

	inline FCoverHandle ConvertEntityId(FCEntityId Id)
	{
		return FCoverHandle(Id.value);
	}
}