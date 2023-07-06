// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_StartShotAI.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API UBTT_StartShotAI : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_StartShotAI();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
