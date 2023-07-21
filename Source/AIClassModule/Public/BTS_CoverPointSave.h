// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CoverPointSave.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API UBTS_CoverPointSave : public UBTService
{
	GENERATED_BODY()	
public:
	UBTS_CoverPointSave();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
