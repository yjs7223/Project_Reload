// Copyright (c), Sami Kangasmaa 2022

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"

#include "AIController.h"
#include "CoverSystemPublicData.h"
#include "CoverSystem.h"

#include "BTService_OccupyCoversInPlace.generated.h"

class UBehaviorTree;
class AActor;
class UBlackboardComponent;

struct FOccupyCoverInPlaceServiceMemory
{
	TSet<FCoverHandle> OccupiedHandles;
};

/**
 * Service that automatically occupies all covers inside character bounds
 */
UCLASS()
class AICOVERSYSTEM_API UBTService_OccupyCoversInPlace : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTService_OccupyCoversInPlace();

	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FOccupyCoverInPlaceServiceMemory); }

	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:

	// Should the service use character capsule bounds when occupying covers in pawn location?
	UPROPERTY(EditDefaultsOnly, Category = "Service")
	bool bUseCharacterCapsuleBounds = true;

	// Bounds to use as fallback or when bUseCapsuleBounds is set to false
	UPROPERTY(EditDefaultsOnly, Category = "Service")
	FVector BoundExtents = FVector(32.0f, 32.0f, 32.0f);

private:

	void OccupyCoversInBounds(AAIController* Controller, FOccupyCoverInPlaceServiceMemory* Memory);
	void QueryCoverHandles(AAIController* Controller, TArray<FCoverHandle>& Handles) const;
	void ReleaseOccupiedCovers(AAIController* Controller, FOccupyCoverInPlaceServiceMemory* Memory, TArray<FCoverHandle>* IgnoreHandles = nullptr);
};
