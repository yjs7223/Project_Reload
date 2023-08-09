// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_NomalCoverRequest.h"
#include "AI_Controller.h"
#include "AICommander.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_NomalCoverRequest::UBTT_NomalCoverRequest()
{
	NodeName = TEXT("NomalCoverRequest");
}

EBTNodeResult::Type UBTT_NomalCoverRequest::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, OwnerComp.GetAIOwner()->GetName());
	OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool("OrderWait", true);
	if (Cast<AAI_Controller>(OwnerComp.GetAIOwner())->commander)
	{
		AIController = Cast<AAI_Controller>(OwnerComp.GetAIOwner())->commander;
	}
	if (AIController)
	{
		if (AIController->GetBlackboardComponent())
		{
			AIController->GetBlackboardComponent()->SetValueAsBool("CmdAI_OrderWait", true);
		}
	}
	
	return EBTNodeResult::Succeeded;

}
