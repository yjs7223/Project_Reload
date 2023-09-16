// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"
#include "UObject/ConstructorHelpers.h" 
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

ABossAIController::ABossAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> 
		BTObject(TEXT("BehaviorTree'/Game/Boss/BehaviorTree/BT_Boss.BT_Boss'"));

	if (BTObject.Succeeded())
	{
		btree = BTObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("Set BehaviorTree Succeed!"));
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData>
		BBObject(TEXT("BlackboardData'/Game/Boss/BehaviorTree/BB_Boss.BB_Boss'"));

	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
		btree->BlackboardAsset = BBAsset;
		UE_LOG(LogTemp, Warning, TEXT("Set BlackBoard Succeed!"));
	}
}

void ABossAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBTT();
}

void ABossAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABossAIController::RunBTT()
{
	RunBehaviorTree(btree);
}
