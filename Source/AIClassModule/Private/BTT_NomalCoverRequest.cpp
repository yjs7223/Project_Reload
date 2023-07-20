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
	BlackboardComponent = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	BlackboardComponent->SetValueAsBool("OrderWait", true);
	AIController = nullptr;
	ACharacter = Cast<AAICharacter>(Cast<AAICommander>(commander));
	if (ACharacter)
	{
		AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ACharacter)->GetController());
	}
	if (AIController)
	{
		if (AIController->BlackboardComponent)
		{
			BlackboardComponent = AIController->BlackboardComponent;
			if (BlackboardComponent->GetValueAsBool("Cmd_OrderWait"))
			{
				BlackboardComponent->SetValueAsBool("Cmd_OrderWait", false);
			}
		}
	}
	BlackboardComponent = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	int delay = 1;
	for (int i = 0; i < delay; i++)
	{
		if (BlackboardComponent->GetValueAsBool("OrderWait"))
		{
			delay++;
		}
		else
		{
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
