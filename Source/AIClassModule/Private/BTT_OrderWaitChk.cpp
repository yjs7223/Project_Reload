// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_OrderWaitChk.h"
#include "AI_Controller.h"
#include "AICommander.h"
#include "AICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_OrderWaitChk::UBTT_OrderWaitChk()
{
	NodeName = TEXT("OrderWaitChk");
	orderchk = false;
}

EBTNodeResult::Type UBTT_OrderWaitChk::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!commander)
	{
		commander = Cast<AAICommander>(OwnerComp.GetAIOwner());
	}
	orderchk = false;
	for (auto ai : commander->List_Division)
	{
		AIController = nullptr;
		AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ai.Key)->GetController());
		if (AIController)
		{
			if (AIController->GetBlackboardComponent())
			{
				if (AIController->GetBlackboardComponent()->GetValueAsBool("OrderWait"))
				{
					orderchk = true;
				}
			}
		}
	}
	if (commander && commander->GetBlackboardComponent())
	{
		if (orderchk)
		{
			commander->GetBlackboardComponent()->SetValueAsBool("Cmd_OrderWait", true);
		}
		else
		{
			commander->GetBlackboardComponent()->SetValueAsBool("Cmd_OrderWait", false);
		}
	}
	
	return EBTNodeResult::Succeeded;
}
