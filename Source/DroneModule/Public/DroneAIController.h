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
	/** Component used for moving along a path. */
	/*UPROPERTY(VisibleDefaultsOnly, Category = AI)
		class UPathFollowingComponent* CustomPathFollowingComponent;*/


	//UFUNCTION(BlueprintCallable, Category = "MascotDrone", Meta = (AdvancedDisplay = "bStopOnOverlap,bCanStrafe,bAllowPartialPath"))
	//	EPathFollowingRequestResult::Type CustomMoveToLocation(const FVector& Dest, float AcceptanceRadius = -1, bool bStopOnOverlap = true,
	//		bool bUsePathfinding = true, bool bProjectDestinationToNavigation = false, bool bCanStrafe = true,
	//		TSubclassOf<UNavigationQueryFilter> FilterClass = NULL, bool bAllowPartialPath = true);

};
