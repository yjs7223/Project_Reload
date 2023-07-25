// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SiegeCoverSelection.h"
#include "AI_Controller.h"
#include "AICommander.h"
#include "AICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

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
	commander->SiegeCoverPoint();
	maplistnum = commander->SiegeCoverArray.Num();
	enemyActivenum = 0;
	for (auto ai : commander->List_Division)
	{
		AIController = nullptr;
		ACharacter = Cast<AAICharacter>(ai.Key);
		if (ACharacter)
		{
			AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ACharacter)->GetController());
		}
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
									if (commander->SiegeCoverArray[i] == coverpoint.Value)
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
								AIController->GetBlackboardComponent()->SetValueAsVector("AI_MoveLocation", commander->SiegeCoverArray[i]);
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
