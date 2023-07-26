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
	if (!commander)
	{
		commander = Cast<AAICommander>(OwnerComp.GetAIOwner());
	}
	if (!player)
	{
		player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}
	commander->DetourCoverPoint();
	for (auto ai : commander->List_Division)
	{
		Cast<AAICharacter>(ai.Key)->Detour = false;
	}
	for (auto cover : commander->DetourCoverArray)
	{
		select_ai = nullptr;
		Detourchange = false;
		for (auto ai : commander->List_Division)
		{
			if (!Cast<AAICharacter>(ai.Key)->Detour)
			{
				Detourchange = true;
				if (commander->IsPlayerInsideFanArea(ai.Key->GetActorLocation(), 2000, 160, player->GetActorForwardVector())
					|| !commander->IsPlayerInsideFanArea(ai.Key->GetActorLocation(), 2000, 240, player->GetActorForwardVector()))
				{
					if (*commander->List_Suppression.Find(ai.Value) > 30.0f)
					{
						if (*commander->List_Combat.Find(ai.Value) == ECombat::InCover)
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
		AIController = Cast<AAI_Controller>(Cast<AAICharacter>(select_ai)->GetController());
		if (AIController)
		{
			if (AIController->GetBlackboardComponent())
			{
				AIController->GetBlackboardComponent()->SetValueAsVector("AI_MoveLocation", cover);
				Cast<AAICharacter>(select_ai)->Detour = true;
				commander->List_CoverPoint.Add(*commander->List_Division.Find(select_ai), cover);
			}
		}

	}
	return EBTNodeResult::Succeeded;
}


