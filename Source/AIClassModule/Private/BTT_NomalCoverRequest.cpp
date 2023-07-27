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
	AAIController* test = OwnerComp.GetAIOwner();
	OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool("OrderWait", true);
	if (!AIController)
	{
		AIController = Cast<AAI_Controller>(OwnerComp.GetAIOwner())->commander;
	}
	
	if (AIController)
	{
		if (AIController->GetBlackboardComponent())
		{
			
			if (!AIController->GetBlackboardComponent()->GetValueAsBool("CmdAI_OrderWait"))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "SDADSADADAASDDADADAD");
				AIController->GetBlackboardComponent()->SetValueAsBool("CmdAI_OrderWait", true);
			}
		}
	}
	
	return EBTNodeResult::Succeeded;

}
