//// Copyright, (c) Sami Kangasmaa 2022

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/Tasks/BTTask_RunEQSQuery.h"

#include "CoverSystemVersion.h"
#include "CoverSystem.h"

#include "BTTask_EQS_Query_Cover.generated.h"

/**
 * Runs environment query and stores cover and its location into a separate blackboard keys
 */
UCLASS(BlueprintType, meta = (DisplayName = "Cover EQS Query"))
class AICOVERSYSTEM_API UBTTask_EQS_Query_Cover : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()

public:

	/** get name of selected blackboard key */
	FName GetSelectedLocationBlackboardKey() const;

	/** blackboard key selector. Location of the cover is stored here */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector LocationBlackboardKey;

	UPROPERTY(EditAnywhere, Category = Node, meta = (InlineEditConditionToggle))
	bool bUseBBKey;

	UPROPERTY(Category = EQS, EditAnywhere)
	FEQSParametrizedQueryExecutionRequest EQSRequest;

	UPROPERTY(Category = EQS, EditAnywhere)
	bool bUpdateBBOnFail = false;

	FQueryFinishedSignature QueryFinishedDelegate;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;
	virtual uint16 GetInstanceMemorySize() const override;

	/** finish task */
	void OnQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	/** Convert QueryParams to QueryConfig */
	virtual void PostLoad() override;

#if WITH_EDITOR
	/** prepare query params */
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual FName GetNodeIconName() const override;
#endif

protected:

	/** gather all filters from existing EnvQueryItemTypes */
	void CollectKeyFilters();
	
};

//////////////////////////////////////////////////////////////////////////
// Inlines

FORCEINLINE FName UBTTask_EQS_Query_Cover::GetSelectedLocationBlackboardKey() const
{
	return LocationBlackboardKey.SelectedKeyName;
}