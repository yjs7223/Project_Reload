//// Copyright, (c) Sami Kangasmaa 2022


#include "CoverMemory.h"

FCoverMemory::FCoverMemory(float InOctreeExtent, FVector InOctreeOrigin)
{
	Entities = nullptr;
	Octree = nullptr;
	OctreeExtent = InOctreeExtent;
	OctreeOrigin = InOctreeOrigin;
	Initialize();
}

FCoverMemory::~FCoverMemory()
{
	Release();
}

void FCoverMemory::Initialize()
{
	check(Entities == nullptr);
	check(Octree == nullptr);

	Entities = new TCEntities<FCoverEntity>();
	Octree = new FCoverOctree(OctreeOrigin, OctreeExtent);
}

void FCoverMemory::Release()
{
	if (Octree)
	{
		Octree->Destroy();
		delete Octree;
		Octree = nullptr;
	}

	if (Entities)
	{
		Entities->Reset();
		delete Entities;
		Entities = nullptr;
	}
}

void FCoverMemory::Reset()
{
	Release();
	Initialize();
}

void FCoverMemory::BuildOctree()
{
	check(Octree);
	Octree->Destroy();

	for (const FCoverEntity& Entity : *Entities)
	{
		Octree->AddElement(FCoverOctreeElement(Entity));
	}

	Octree->ShrinkElements();
}