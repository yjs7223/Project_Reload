// Fill out your copyright notice in the Description page of Project Settings.


#include "BossDroneController.h"
#include "UObject/ConstructorHelpers.h" 
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

ABossDroneController::ABossDroneController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
		BTObject(TEXT("BehaviorTree'/Game/Boss/BehaviorTree/Drone/BT_BossDrone.BT_BossDrone'"));

	if (BTObject.Succeeded())
	{
		btree = BTObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("Set Drone BehaviorTree Succeed!"));
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData>
		BBObject(TEXT("BlackboardData'/Game/Boss/BehaviorTree/Drone/BB_BossDrone.BB_BossDrone'"));

	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
		btree->BlackboardAsset = BBAsset;
		UE_LOG(LogTemp, Warning, TEXT("Set Drone BlackBoard Succeed!"));
	}
}

void ABossDroneController::BeginPlay()
{
	Super::BeginPlay();

	RunBTT();
}

void ABossDroneController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABossDroneController::RunBTT()
{
	RunBehaviorTree(btree);
}
