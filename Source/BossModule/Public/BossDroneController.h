// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossDroneController.generated.h"

/**
 * 
 */
UCLASS()
class BOSSMODULE_API ABossDroneController : public AAIController
{
	GENERATED_BODY()

public:
	ABossDroneController();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:

	class UBlackboardData* BBAsset;
	class UBehaviorTree* btree;
	class UBehaviorTreeComponent* behavior_tree_component;

public:

	UFUNCTION(BlueprintCallable, Category = "BossAIController")
		void RunBTT();
	
};
