// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_InCover.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTaskNode_InCover::UBTTaskNode_InCover()
{

	NodeName = TEXT("BTTaskNode_InCover");

}

EBTNodeResult::Type UBTTaskNode_InCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    /*UBlackboardComponent * MyBlackboard = OwnerComp.GetBlackboardComponent();
    MyBlackboard->ClearValue(MyBlackboardKey.SelectedKeyName);
    MyBlackboard->GetValueAsBool(MyBlackboardKey.SelectedKeyName);
    MyBlackboard->SetValueAsBool(MyBlackboardKey.SelectedKeyName, false);*/

    return EBTNodeResult::Succeeded;
}
