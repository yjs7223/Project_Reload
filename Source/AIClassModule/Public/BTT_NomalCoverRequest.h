// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_NomalCoverRequest.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API UBTT_NomalCoverRequest : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_NomalCoverRequest();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_NomalCoverRequest)
		UBlackboardComponent* BlackboardComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_NomalCoverRequest)
		class AAICommander* AIController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_NomalCoverRequest)
		class AAICharacter* ACharacter;
};
