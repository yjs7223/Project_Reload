// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_DetourCoverSelection.h"
#include "AI_Controller.h"
#include "AICommander.h"
#include "AICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_DetourCoverSelection::UBTT_DetourCoverSelection()
{
	NodeName = TEXT("DetourCoverSelection");
}

EBTNodeResult::Type UBTT_DetourCoverSelection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	for (auto ai : Cast<AAICommander>(OwnerComp.GetAIOwner())->List_Division)
	{
		Cast<AAICharacter>(ai.Key)->Detour = false;
	}
	for (auto cover : Cast<AAICommander>(OwnerComp.GetAIOwner())->DetourCoverArray)
	{
		select_ai = nullptr;
		Detourchange = false;
		for (auto ai : Cast<AAICommander>(OwnerComp.GetAIOwner())->List_Division)
		{
			if (!Cast<AAICharacter>(ai.Key)->Detour)
			{
				Detourchange = true;
				if (Cast<AAICommander>(OwnerComp.GetAIOwner())->IsPlayerInsideFanArea(ai.Key->GetActorLocation(), 2000, 160, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorForwardVector())
					|| !Cast<AAICommander>(OwnerComp.GetAIOwner())->IsPlayerInsideFanArea(ai.Key->GetActorLocation(), 2000, 240, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorForwardVector()))
				{
					if (*Cast<AAICommander>(OwnerComp.GetAIOwner())->List_Suppression.Find(ai.Value) > 30.0f)
					{
						if (*Cast<AAICommander>(OwnerComp.GetAIOwner())->List_Combat.Find(ai.Value) == ECombat::InCover)
						{
							if (select_ai == nullptr)
							{
								Dis_Loc = FVector::Distance(cover, ai.Key->GetActorLocation());
								select_ai = ai.Key;
							}
							else
							{
								if (Dis_Loc > FVector::Distance(cover, ai.Key->GetActorLocation()))
								{
									Dis_Loc = FVector::Distance(cover, ai.Key->GetActorLocation());
									select_ai = ai.Key;
								}
							}
						}
					}
				}
			}
		}
		if (!Detourchange)
		{
			return EBTNodeResult::Succeeded;
		}
		AIController = nullptr;
		ACharacter = Cast<AAICharacter>(select_ai);
		if (ACharacter)
		{
			AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ACharacter)->GetController());
		}
		if (AIController)
		{
			if (AIController->GetBlackboardComponent())
			{
				AIController->GetBlackboardComponent()->SetValueAsVector("AI_MoveLocation", cover);
				Cast<AAICharacter>(select_ai)->Detour = true;
			}
		}

	}
	return EBTNodeResult::Succeeded;
}


