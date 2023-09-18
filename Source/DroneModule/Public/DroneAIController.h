// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"


#include "DroneAIController.generated.h"

/**
 * 
 */
UCLASS()
class DRONEMODULE_API ADroneAIController : public AAIController
{
	GENERATED_BODY()
public:
	ADroneAIController();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;


public:
		class UBehaviorTree* btree;


public:

	UFUNCTION(BlueprintCallable, Category = "DroneAI_Controller")
		void RunBTT();


public:

	UFUNCTION(BlueprintCallable, Category = "MascotDrone")
		bool DroneMoveTo(FVector p_vec, float p_radius);

};
