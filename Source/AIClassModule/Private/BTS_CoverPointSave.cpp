// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CoverPointSave.h"
#include "AICommander.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_CoverPointSave::UBTS_CoverPointSave()
{


}

void UBTS_CoverPointSave::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World) return;

}
