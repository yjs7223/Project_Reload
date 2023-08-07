// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SiegeCoverSelection.h"
#include "AI_Controller.h"
#include "AICommander.h"
#include "AICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SubEncounterSpace.h"

UBTT_SiegeCoverSelection::UBTT_SiegeCoverSelection()
{
	NodeName = TEXT("SiegeCoverSelection");
	maplistnum = 0;
	enemyActivenum = 0;
}

EBTNodeResult::Type UBTT_SiegeCoverSelection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!commander)
	{
		commander = Cast<AAICommander>(OwnerComp.GetAIOwner());
	}
	if (commander->SiegeCoverArray.IsEmpty())
	{
		return EBTNodeResult::Succeeded;
	}
	commander->SiegeCoverPoint();
	maplistnum = commander->SiegeCoverArray.Num();
	enemyActivenum = 0;
	for (auto ai : commander->List_Division)
	{
		AIController = nullptr;
		AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ai.Key)->GetController());
		
		if (AIController)
		{
			if (AIController->GetBlackboardComponent())
			{
				if(AIController->GetBlackboardComponent()->GetValueAsEnum("Combat") == 4)
				{
					enemyActivenum++;
					if (enemyActivenum <= maplistnum)
					{
						arraysame = false;
						int num = 1;
						for (int i = 0; i < num; i++)
						{
							for (auto coverpoint : commander->List_CoverPoint)
							{
								if (coverpoint.Key != ai.Value)
								{
									if (FVector::Distance(commander->SiegeCoverArray[i], coverpoint.Value) < 200)
									{
										arraysame = true;
									}
								}
							}
							for (auto subAi : commander->Now_suben->AIArray)
							{
								if (subAi != ai.Key)
								{
									if (!Cast<AAI_Controller>(Cast<AAICharacter>(subAi)->GetController()))
									{
										continue;
									}
									AAI_Controller* sub_aic = Cast<AAI_Controller>(Cast<AAICharacter>(subAi)->GetController());
									if (FVector::Distance(commander->SiegeCoverArray[i], sub_aic->GetBlackboardComponent()->GetValueAsVector("AI_MoveLocation")) < 200)
									{
										arraysame = true;
									}
								}
							}
							if (arraysame)
							{
								num++;
								if (num >= commander->SiegeCoverArray.Num())
								{
									return EBTNodeResult::Succeeded;
								}
							}
							else if (!arraysame)
							{
								AIController->GetBlackboardComponent()->SetValueAsVector("AI_CoverLocation", commander->SiegeCoverArray[i]);
								commander->List_CoverPoint.Add(ai.Value, commander->SiegeCoverArray[i]);
							}
						}
					}
					else
					{
						return EBTNodeResult::Succeeded;
					}
				}

			}
		}
	}
	return EBTNodeResult::Succeeded;
}
