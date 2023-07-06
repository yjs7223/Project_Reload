// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_StopShotAI.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API UBTT_StopShotAI : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_StopShotAI();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
