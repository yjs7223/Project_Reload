// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_InCover.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseAICtr.h"


UBTTaskNode_InCover::UBTTaskNode_InCover()
{

	NodeName = TEXT("BTTaskNode_InCover");

}

EBTNodeResult::Type UBTTaskNode_InCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    //auto Controller = Cast<AMeleeEnemyAIController>(OwnerComp.GetAIOwner());
    //GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "TTTTTTTT");


    OwnerComp.GetBlackboardComponent()->SetValueAsVector(ABaseAICtr::Key_CoverLocation, FVector(0, 0, 0));
        //OwnerComp.GetBlackboardComponent()->GetValueAsVector(ABaseAICtr::Key_Cover));

    return EBTNodeResult::Succeeded;
}
