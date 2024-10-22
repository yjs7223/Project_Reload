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
	if (Cast<AAI_Controller>(OwnerComp.GetAIOwner())->commander)
	{
		commander = Cast<AAI_Controller>(OwnerComp.GetAIOwner())->commander;
	}
	if (commander)
	{
		if (commander->GetBlackboardComponent())
		{
			commander->CoverPointEnemy();
			OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool("OrderWait", true);
			commander->GetBlackboardComponent()->SetValueAsBool("CmdAI_OrderWait", true);
		}
	}
	
	return EBTNodeResult::Succeeded;

}
