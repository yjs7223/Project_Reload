//// Copyright, (c) Sami Kangasmaa 2022

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"

#include "AIController.h"
#include "CoverSystemPublicData.h"
#include "CoverSystem.h"

#include "BTService_OccupyCover.generated.h"

class UBehaviorTree;
class AActor;
class UBlackboardComponent;

struct FOccupyCoverServiceMemory
{
	FCoverHandle Cover;
	FDelegateHandle BBObserverDelegateHandle;
	void Reset()
	{
		Cover = FCoverHandle();
	}
};

/**
 * Occupies cover for an AI as long as the service is active
 */
UCLASS(BlueprintType, hidecategories = (Service))
class AICOVERSYSTEM_API UBTService_OccupyCover : public UBTService_BlackboardBase
{
	GENERATED_BODY()

protected:

	/** if the selected cover blackboard value changes, should the occupy state updated? */
	UPROPERTY(Category = Blackboard, EditAnywhere)
	bool bObserveBlackboardValue = true;

public:

	UBTService_OccupyCover(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual FString GetStaticDescription() const override;

	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FOccupyCoverServiceMemory); }
	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	EBlackboardNotificationResult OnBlackboardValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);
};
