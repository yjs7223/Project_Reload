// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAICtr.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"


const FName ABaseAICtr::Key_Cover(TEXT("Key_Cover"));
const FName ABaseAICtr::Key_CoverLocation(TEXT("Key_CoverLocation"));

ABaseAICtr::ABaseAICtr()
{
	PrimaryActorTick.bCanEverTick = true;


	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/_sjs/BB_BaseAI.BB_BaseAI'"));
	if (BBObject.Succeeded())
		BBAsset = BBObject.Object;


	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/_sjs/BT_BaseAI.BT_BaseAI'"));
	if (BTObject.Succeeded())
		BTAsset = BTObject.Object;
}



void ABaseAICtr::BeginPlay()
{

	Super::BeginPlay();
}

void ABaseAICtr::OnPossess(APawn* pPawn)
{
	Super::OnPossess(pPawn);

	UBlackboardComponent* BlackboardComp = Blackboard;
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		if (!RunBehaviorTree(BTAsset))
			UE_LOG(LogTemp, Warning, TEXT("AIController couldn't run behavior tree!"));
	}
}
