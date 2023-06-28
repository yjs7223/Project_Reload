// Copyright (c), Sami Kangasmaa 2022

#pragma once

#include "CoreMinimal.h"
#include "CoverSystemVersion.h"

#include "BehaviorTree/Services/BTService_BlackboardBase.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CoverSystem.h"

#include "BTService_EQS_Query_Cover.generated.h"

class UBehaviorTree;

struct FBTCoverEQSServiceMemory
{
	int32 RequestID;
};

/**
 * Runs environment query in intervals and stores cover and its location into a separate blackboard keys
 */
UCLASS(BlueprintType)
class AICOVERSYSTEM_API UBTService_EQS_Query_Cover : public UBTService_BlackboardBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(Category = EQS, EditAnywhere)
	FEQSParametrizedQueryExecutionRequest EQSRequest;

	UPROPERTY(Category = EQS, EditAnywhere)
	bool bUpdateBBOnFail = false;

	FQueryFinishedSignature QueryFinishedDelegate;

public:

	UBTService_EQS_Query_Cover();

	/** get name of selected blackboard key */
	FName GetSelectedLocationBlackboardKey() const;

	/** blackboard key selector. Location of the cover is stored here */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector LocationBlackboardKey;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTCoverEQSServiceMemory); }

	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual void CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	void OnQueryFinished(TSharedPtr<FEnvQueryResult> Result);
	
};

//////////////////////////////////////////////////////////////////////////
// Inlines

FORCEINLINE FName UBTService_EQS_Query_Cover::GetSelectedLocationBlackboardKey() const
{
	return LocationBlackboardKey.SelectedKeyName;
}