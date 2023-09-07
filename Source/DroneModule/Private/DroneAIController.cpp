// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneAIController.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

ADroneAIController::ADroneAIController()
{
	//bt Setting
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
		BTObject(TEXT("BehaviorTree'/Game/_sjs/Drone/BT_DroneAI.BT_DroneAI'"));

	if (BTObject.Succeeded())
	{
		btree = BTObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("BehaviorTree Succeed!"));
	}




}

void ADroneAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBTT();
}

void ADroneAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ADroneAIController::RunBTT()
{
	RunBehaviorTree(btree);
}
