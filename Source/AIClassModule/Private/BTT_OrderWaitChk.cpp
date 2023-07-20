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
	orderchk = false;
	for (auto ai : Cast<AAICommander>(OwnerComp.GetAIOwner())->List_Division)
	{
		AIController = nullptr;
		ACharacter = Cast<AAICharacter>(ai.Key);
		if (ACharacter)
		{
			AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ACharacter)->GetController());
		}
		if (AIController)
		{
			if (AIController->BlackboardComponent)
			{
				BlackboardComponent = AIController->BlackboardComponent;
				if (BlackboardComponent->GetValueAsBool("OrderWait"))
				{
					orderchk = true;
				}
			}
		}
	}
	AIController = nullptr;
	ACharacter = Cast<AAICharacter>(Cast<AAICommander>(OwnerComp.GetAIOwner()));
	if (ACharacter)
	{
		AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ACharacter)->GetController());
	}
	if (AIController)
	{
		if (AIController->BlackboardComponent)
		{
			BlackboardComponent = AIController->BlackboardComponent;
			if (orderchk)
			{
				BlackboardComponent->SetValueAsBool("Cmd_OrderWait", true);
			}
			else
			{
				BlackboardComponent->SetValueAsBool("Cmd_OrderWait", false);
			}
		}
	}
	
	return EBTNodeResult::Succeeded;
}
