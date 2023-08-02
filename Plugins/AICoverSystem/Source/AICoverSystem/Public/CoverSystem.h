//// Copyright, (c) Sami Kangasmaa 2022

#pragma once

#include "CoreMinimal.h"
#include "CoverSystemVersion.h"
#include "GameFramework/Info.h"
#include "AIController.h"
#include "CoverSystemPublicData.h"
#include "CoverSystem.generated.h"

class FCoverSystemProxy;


/**
 * Manager class of the cover system.
 * Place this into the persistent level
 */
UCLASS(Blueprintable)
class AICOVERSYSTEM_API ACoverSystem : public AInfo
{
	GENERATED_BODY()
	
public:

	ACoverSystem();

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;
	virtual void BeginDestroy() override;
	virtual void Tick(float DeltaTime) override;

private:

	UFUNCTION()
	void OnNavigationGenerationFinished(class ANavigationData* NavData);

public:

	/**
	 * Returns cover system from the level.
	 */
	UFUNCTION(BlueprintCallable, Category = "Cover System", meta =(WorldContext="WorldContext"))
	static ACoverSystem* GetCoverSystem(const UObject* WorldContext);

	/**
	 * Launches generation process of covers.
	 * Runs in async mode by default, so the result are not available immediately.
	 * To access generated data immediately, use sync mode (bAsync=false)
	 * Always runs in sync mode, if bEnableAsyncMode is set to false
	 * NOTE! Sync mode will not run, if there's already async build task running.
	 */
	UFUNCTION(BlueprintCallable, Category = "Cover System")
	void GenerateCovers(bool bForceRegenerate = false, bool bAsync = true, bool bDeferGenerationIfBusy = true);

	/**
	 * Rebases the origin of the system.
	 * Use this in large worlds to move cover generation with the player
	 */
	UFUNCTION(BlueprintCallable, Category = "Cover System")
	void RebaseOrigin(const FVector& NewOrigin, bool bRegenerate = true);

	/**
	 * Outputs data of given cover by handle.
	 * Return true if the data was found and is valid
	 */
	UFUNCTION(BlueprintCallable, Category = "Cover System")
	bool GetCoverData(const FCoverHandle& CoverHandle, FCoverData& OutData) const;

	/**
	 * Outputs all cover handles that are inside given bounds
	 */
	void GetCoversWithinBounds(const class FBoxCenterAndExtent& InBounds, TArray<FCoverHandle>& OutCoverHandles) const;

	/**
	 * Outputs all covers that are inside given bounds
	 */
	void GetCoversAndDataWithinBounds(const class FBoxCenterAndExtent& InBounds, TArray<FCover>& OutCovers) const;

	/**
	 * Outputs all cover handles that are inside given bounds
	 */
	UFUNCTION(BlueprintCallable, Category = "Cover System")
	void GetCoversWithinBounds(const FBoxSphereBounds& InBounds, TArray<FCoverHandle>& OutCoverHandles) const;

	/**
	 * Runs a query to octree and retreves list of covers from entity database that are inside given bounds
	 */
	UFUNCTION(BlueprintCallable, Category = "Cover System")
	void GetCoverDataWithinBounds(const FBoxSphereBounds& InBounds, TArray<FCover>& OutCovers) const;

	/*
	* Occupies cover for given controller
	* AI's can test, if certain cover is already in use
	*/
	UFUNCTION(BlueprintCallable, Category = "Cover System")
	bool OccupyCover(AController* Controller, const FCoverHandle& CoverHandle);

	/**
	 * Releases cover, so it is no more occupied
	 */
	UFUNCTION(BlueprintCallable, Category = "Cover System")
	void UnOccupyCover(const FCoverHandle& CoverHandle);

	/**
	 * Releases cover, so it is no more occupied
	 * Specialized version of UnOccupyCover which unoccupies the cover only, if it is already occupied by given controller
	 */
	UFUNCTION(BlueprintCallable, Category = "Cover System")
	void UnOccupyCoverFromController(const FCoverHandle& CoverHandle, AController* Controller);

	/**
	 * Returns a controller that is currently occupying given cover (null, if the cover is empty or invalid)
	 */
	UFUNCTION(BlueprintPure, Category = "Cover System")
	AController* GetOccupyingController(const FCoverHandle& CoverHandle) const;

	/*
	* Checks if cover is occupied
	*/
	UFUNCTION(BlueprintPure, Category = "Cover System")
	bool IsCoverOccupied(const FCoverHandle& CoverHandle) const;

	/**
	 * Checks if async worker task is currently running
	 */
	UFUNCTION(BlueprintPure, Category = "Cover System")
	bool IsAsyncWorkerRunning() const;

protected:

	// Should generation process started on begin play?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	bool bGenerateOnBeginPlay = true;


	// Should generation process started after navigation rebuild?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	bool bGenerateOnNavigationRebuild = false;

	// Should async build task used to generate covers?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	bool bEnableAsyncMode = true;

	// Parameters for cover generation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (ExposeOnSpawn))
	FCoverBuildParams BuildParams;

	/*
	* Trace channel to use when finding blocking geometry
	* To customize cover generation, you can create a new trace channel in project settings
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_WorldStatic;
	
	// Toggles debug draw on/off
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bDebugDraw = false;

	// Culling distance for debug draw
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	float DebugDrawDistance = 80000.0f;

	// Should debug draw octree root node bounds?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (EditCondition="bDebugDraw"))
	bool bDrawOctreeBounds = false;

private:

	// Pointer to pure C++ proxy object that manages build tasks and cover memory
	FCoverSystemProxy* Proxy;

	// Mapping for occupied covers by controllers
	TMap<FCoverHandle, TWeakObjectPtr<AController>> OccupiedCovers;

	// Is generation currently dirty and requires update once available?
	bool bDirtyGeneration = false;

	// Calculates octree origin for generation
	FVector CalculateOctreeOrigin() const;

private:

	void DebugDraw(UWorld* World, FCoverSystemProxy* InProxy, bool bPersistent = false);
	bool bFlushPreviewDebug = false;

public:
#if WITH_EDITOR

	/**
	 * Cleans up preview covers from debug draw
	 */
	UFUNCTION(Category = "Debug", meta = (CallInEditor = "true"))
	void CleanPreview();

	/**
	 * Runs generation process in the editor to preview covers
	 */
	UFUNCTION(Category = "Debug", meta = (CallInEditor = "true"))
	void PreviewGenerate();

#endif
};
