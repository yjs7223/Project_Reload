// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SpiderSpawn.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API UBTT_SpiderSpawn : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_SpiderSpawn();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
