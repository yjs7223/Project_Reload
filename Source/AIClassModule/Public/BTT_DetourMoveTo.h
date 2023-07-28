// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_DetourMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API UBTT_DetourMoveTo : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_DetourMoveTo();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
