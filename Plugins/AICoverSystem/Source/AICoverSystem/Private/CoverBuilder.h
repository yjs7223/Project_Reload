//// Copyright, (c) Sami Kangasmaa 2022

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "CoverSystemPublicData.h"
#include "CoverMemory.h"

#include "Detour/DetourNavMesh.h"
#include "Navmesh/RecastHelpers.h"

/**
 * Builder class for the covers.
 * Initializes a new cover memory and performs build operation
 */
class FCoverBuilder
{
public:

	FCoverBuilder();
	~FCoverBuilder();

	bool Build(FCoverMemory* Memory, UWorld* World, const ARecastNavMesh* NavMesh, const FCoverBuildParams& Params);
	void GetNavTilesInOctree(const dtNavMesh* dtNav, const FBoxCenterAndExtent& OctreeExtent, TArray<int32>& TileIndices) const;
	void GetNavEdges(const dtNavMesh* dtNav, const FBoxCenterAndExtent& OctreeExtent, const TArray<int32>& TileIndices, TArray<FVector>& OutEdgeVertices) const;
	void GetNavTileEdges(const dtNavMesh* dtNav, const dtMeshTile& Tile, const FBoxCenterAndExtent& OctreeExtent, bool bInternalEdges, bool bNavMeshEdges, TArray<FVector>& InternalEdgeVerts, TArray<FVector>& NavMeshEdgeVerts) const;
};
