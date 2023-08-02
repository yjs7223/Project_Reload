//// Copyright, (c) Sami Kangasmaa 2022


#include "CoverBuilder.h"
#include "CoverSolver.h"
#include "DrawDebugHelpers.h"
#include "Deams51Algorithms.h"
#include "CoverSystemVersion.h"

FCoverBuilder::FCoverBuilder() {}

FCoverBuilder::~FCoverBuilder() {}

bool FCoverBuilder::Build(FCoverMemory* Memory, UWorld* World, const ARecastNavMesh* NavMesh, const FCoverBuildParams& Params)
{
	/*
	* Note here!
	* Use world ptr only for read operations! - We are running in background thread.
	* Proxy should ensure that this task completes in case the world is being destroyed, so it should be valid during the whole task execution
	*/

	if (!ensure(Memory))
	{
		return false;
	}

	if (!ensure(World))
	{
		return false;
	}

	if (!ensure(NavMesh))
	{
		return false;
	}

	check(Memory->GetEntities());
	check(Memory->GetOctree());

	// Collect navigation geometry data
	NavMesh->BeginBatchQuery();

	const dtNavMesh* dtNav = NavMesh->GetRecastMesh();
	if (!ensure(dtNav))
	{
		NavMesh->FinishBatchQuery();
		return false;
	}

	// Find tiles inside octree bounds
	TArray<int32> TileIndicies;
	GetNavTilesInOctree(dtNav, Memory->GetOctree()->GetRootBounds(), TileIndicies);

	// Get nav edge geometry
	TArray<FVector> NavEdges;
	GetNavEdges(dtNav, Memory->GetOctree()->GetRootBounds(), TileIndicies, NavEdges);

	NavMesh->FinishBatchQuery();

	Deams51::FDeams51CoverGenerator::Build_AnalyzeMeshData(Memory, World, NavEdges, Params, FCoverSolver::GetCoverTraceChannel());

	Memory->GetOctree()->ShrinkElements();

	// Build success
	return true;
}

void FCoverBuilder::GetNavTilesInOctree(const dtNavMesh* dtNav, const FBoxCenterAndExtent& OctreeExtent, TArray<int32>& TileIndices) const
{
#if COVER_ENGINE_VERSION >= 500
	const FVector::FReal* NavMeshOrigin = dtNav->getParams()->orig;
	const FVector::FReal TileSize = dtNav->getParams()->tileWidth;
#else
	const float* NavMeshOrigin = dtNav->getParams()->orig;
	const float TileSize = dtNav->getParams()->tileWidth;
#endif
	
	const FBox Bounds = OctreeExtent.GetBox();

	TArray<FIntPoint> TileCoords;
	const FBox RcBounds = Unreal2RecastBox(Bounds);
	const int32 XMin = FMath::FloorToInt((RcBounds.Min.X - NavMeshOrigin[0]) / TileSize);
	const int32 XMax = FMath::FloorToInt((RcBounds.Max.X - NavMeshOrigin[0]) / TileSize);
	const int32 YMin = FMath::FloorToInt((RcBounds.Min.Z - NavMeshOrigin[2]) / TileSize);
	const int32 YMax = FMath::FloorToInt((RcBounds.Max.Z - NavMeshOrigin[2]) / TileSize);
	for (int32 y = YMin; y <= YMax; ++y)
	{
		for (int32 x = XMin; x <= XMax; ++x)
		{
			TileCoords.Add(FIntPoint(x, y));
		}
	}

	TileIndices.Reserve(TileCoords.Num() * 3);

	TArray<const dtMeshTile*> MeshTiles;
	MeshTiles.Reserve(3);

	for (const FIntPoint& TileCoord : TileCoords)
	{
		int32 MaxTiles = dtNav->getTileCountAt(TileCoord.X, TileCoord.Y);
		if (MaxTiles > 0)
		{
			MeshTiles.SetNumZeroed(MaxTiles, false);

			const int32 MeshTilesCount = dtNav->getTilesAt(TileCoord.X, TileCoord.Y, MeshTiles.GetData(), MaxTiles);
			for (int32 i = 0; i < MeshTilesCount; ++i)
			{
				const dtMeshTile* MeshTile = MeshTiles[i];
				dtTileRef TileRef = dtNav->getTileRef(MeshTile);
				if (TileRef)
				{
					// Consider only mesh tiles that actually belong to a requested bounds
					FBox TileBounds = Recast2UnrealBox(MeshTile->header->bmin, MeshTile->header->bmax);
					if (TileBounds.Intersect(Bounds))
					{
						int32 TileIndex = (int32)dtNav->decodePolyIdTile(TileRef);
						TileIndices.Add(TileIndex);
					}
				}
			}
		}
	}
}

void FCoverBuilder::GetNavEdges(const dtNavMesh* dtNav, const FBoxCenterAndExtent& OctreeExtent, const TArray<int32>& TileIndices, TArray<FVector>& OutEdgeVertices) const
{
	OutEdgeVertices.Reserve(TileIndices.Num() * 4);
	for (int32 TileIdx : TileIndices)
	{
		dtMeshTile const* const Tile = dtNav->getTile(TileIdx);
		dtMeshHeader const* const Header = Tile->header;
		if (Header != nullptr)
		{
			TArray<FVector> InternalEdges;
			GetNavTileEdges(dtNav, *Tile, OctreeExtent, false, true, InternalEdges, OutEdgeVertices);
		}
	}
}

#if COVER_ENGINE_VERSION >= 500
static FORCEINLINE FVector::FReal PointDistToSegment2DSquared(const FVector::FReal* PT, const FVector::FReal* P, const FVector::FReal* Q)
{
	FVector::FReal pqx = Q[0] - P[0];
	FVector::FReal pqz = Q[2] - P[2];
	FVector::FReal dx = PT[0] - P[0];
	FVector::FReal dz = PT[2] - P[2];
	FVector::FReal d = pqx * pqx + pqz * pqz;
	FVector::FReal t = pqx * dx + pqz * dz;
	if (d != 0) t /= d;
	dx = P[0] + t * pqx - PT[0];
	dz = P[2] + t * pqz - PT[2];
	return dx * dx + dz * dz;
}
#else
static FORCEINLINE float PointDistToSegment2DSquared(const float* PT, const float* P, const float* Q)
{
	float pqx = Q[0] - P[0];
	float pqz = Q[2] - P[2];
	float dx = PT[0] - P[0];
	float dz = PT[2] - P[2];
	float d = pqx * pqx + pqz * pqz;
	float t = pqx * dx + pqz * dz;
	if (d != 0) t /= d;
	dx = P[0] + t * pqx - PT[0];
	dz = P[2] + t * pqz - PT[2];
	return dx * dx + dz * dz;
}
#endif

void FCoverBuilder::GetNavTileEdges(const dtNavMesh* dtNav, const dtMeshTile& Tile, const FBoxCenterAndExtent& OctreeExtent, bool bInternalEdges, bool bNavMeshEdges, TArray<FVector>& InternalEdgeVerts, TArray<FVector>& NavMeshEdgeVerts) const
{
#if COVER_ENGINE_VERSION >= 500
	static const FVector::FReal thr = FMath::Square(0.01f);
#else
	static const float thr = FMath::Square(0.01f);
#endif

	ensure(bInternalEdges || bNavMeshEdges);
	const bool bExportAllEdges = bInternalEdges && !bNavMeshEdges;

	for (int i = 0; i < Tile.header->polyCount; ++i)
	{
		const dtPoly* Poly = &Tile.polys[i];

		if (Poly->getType() != DT_POLYTYPE_GROUND)
		{
			continue;
		}

		const dtPolyDetail* pd = &Tile.detailMeshes[i];
		for (int j = 0, nj = (int)Poly->vertCount; j < nj; ++j)
		{
			bool bIsExternal = !bExportAllEdges && (Poly->neis[j] == 0 || Poly->neis[j] & DT_EXT_LINK);
			bool bIsConnected = !bIsExternal;

			if (Poly->getArea() == RECAST_NULL_AREA)
			{
				if (Poly->neis[j] && !(Poly->neis[j] & DT_EXT_LINK) &&
					Poly->neis[j] <= Tile.header->offMeshBase &&
					Tile.polys[Poly->neis[j] - 1].getArea() != RECAST_NULL_AREA)
				{
					bIsExternal = true;
					bIsConnected = false;
				}
				else if (Poly->neis[j] == 0)
				{
					bIsExternal = true;
					bIsConnected = false;
				}
			}
			else if (bIsExternal)
			{
				unsigned int k = Poly->firstLink;
				while (k != DT_NULL_LINK)
				{
					const dtLink& link = dtNav->getLink(&Tile, k);
					k = link.next;

					if (link.edge == j)
					{
						bIsConnected = true;
						break;
					}
				}
			}

			TArray<FVector>* EdgeVerts = bInternalEdges && bIsConnected ? &InternalEdgeVerts
				: (bNavMeshEdges && bIsExternal && !bIsConnected ? &NavMeshEdgeVerts : NULL);
			if (EdgeVerts == NULL)
			{
				continue;
			}

#if COVER_ENGINE_VERSION >= 500
			const FVector::FReal* V0 = &Tile.verts[Poly->verts[j] * 3];
			const FVector::FReal* V1 = &Tile.verts[Poly->verts[(j + 1) % nj] * 3];
#else
			const float* V0 = &Tile.verts[Poly->verts[j] * 3];
			const float* V1 = &Tile.verts[Poly->verts[(j + 1) % nj] * 3];
#endif

			for (int32 k = 0; k < pd->triCount; ++k)
			{
				const unsigned char* t = &(Tile.detailTris[(pd->triBase + k) * 4]);
#if COVER_ENGINE_VERSION >= 500
				const FVector::FReal* tv[3];
#else
				const float* tv[3];
#endif

				for (int32 m = 0; m < 3; ++m)
				{
					if (t[m] < Poly->vertCount)
					{
						tv[m] = &Tile.verts[Poly->verts[t[m]] * 3];
					}
					else
					{
						tv[m] = &Tile.detailVerts[(pd->vertBase + (t[m] - Poly->vertCount)) * 3];
					}
				}
				for (int m = 0, n = 2; m < 3; n = m++)
				{
					if (((t[3] >> (n * 2)) & 0x3) == 0)
					{
						continue;	// Skip inner detail edges.
					}

					if (PointDistToSegment2DSquared(tv[n], V0, V1) < thr && PointDistToSegment2DSquared(tv[m], V0, V1) < thr)
					{
						const FVector Vert0 = Recast2UnrealPoint(tv[n]);
						const FVector Vert1 = Recast2UnrealPoint(tv[m]);

						FBoxCenterAndExtent Ext0(Vert0, FVector(1.0f));
						FBoxCenterAndExtent Ext1(Vert1, FVector(1.0f));
						if (Intersect(OctreeExtent, Ext0) && Intersect(OctreeExtent, Ext1))
						{
							int32 const AddIdx = (*EdgeVerts).AddZeroed(2);
							(*EdgeVerts)[AddIdx] = Vert0;
							(*EdgeVerts)[AddIdx + 1] = Vert1;
						}
					}
				}
			}
		}
	}
}