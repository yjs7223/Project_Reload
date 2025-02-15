// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_MovePossible.h"
#include "AI_Controller.h"
#include "AICommander.h"
#include "AICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_MovePossible::UBTT_MovePossible()
{
	NodeName = TEXT("MovePossible");
}

EBTNodeResult::Type UBTT_MovePossible::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;
	
	if (!aic)
	{
		aic = Cast<AAI_Controller>(OwnerComp.GetAIOwner());
	}
	if (!player)
	{
		player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}
	aic->GetBlackboardComponent()->SetValueAsFloat("AI_Dist",
		FVector::Dist(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), player->GetActorLocation()));
	if (aic->GetBlackboardComponent()->GetValueAsFloat("Sup_TotalPoint") > 30.0f)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		dist = aic->GetBlackboardComponent()->GetValueAsFloat("AI_Dist");
		for (auto ai : aic->commander->List_Division)
		{
			if (ai.Key != OwnerComp.GetAIOwner()->GetPawn())
			{
				if (dist < aic->GetBlackboardComponent()->GetValueAsFloat("AI_Dist"))
				{
					return EBTNodeResult::Failed;
				}	
			}
		}
		return EBTNodeResult::Succeeded;
	}
		
	return EBTNodeResult::Failed;
}
