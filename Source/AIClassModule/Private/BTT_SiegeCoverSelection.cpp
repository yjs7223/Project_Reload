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
	if (Cast<AAICharacter>(OwnerComp.GetAIOwner())->type == Enemy_Name::SNIPER || Cast<AAICharacter>(OwnerComp.GetAIOwner())->type == Enemy_Name::HEAVY)
	{
		return EBTNodeResult::Succeeded;
	}
	commander->SiegeCoverPoint();
	maplistnum = commander->SiegeCoverArray.Num();
	enemyActivenum = 0;
	for (auto ai : commander->List_Division)
	{
		AAICharacter* AI = Cast<AAICharacter>(ai.Key);
		if (AI->type == Enemy_Name::RIFLE)
		{
			AIController = nullptr;
			AIController = Cast<AAI_Controller>(AI->GetController());

			if (AIController)
			{
				if (AIController->GetBlackboardComponent())
				{
					if (AIController->GetBlackboardComponent()->GetValueAsEnum("Combat") == 4)
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
								for (auto subAi : commander->List_Division)
								{
									AAICharacter* sub_AI = Cast<AAICharacter>(subAi.Key);
									if (sub_AI->type == Enemy_Name::RIFLE)
									{
										if (subAi.Key != ai.Key)
										{
											if (!Cast<AAI_Controller>(sub_AI->GetController()))
											{
												continue;
											}
											AAI_Controller* sub_aic = Cast<AAI_Controller>(sub_AI->GetController());
											if (FVector::Distance(commander->SiegeCoverArray[i], sub_aic->GetBlackboardComponent()->GetValueAsVector("AI_MoveLocation")) < 200)
											{
												arraysame = true;
											}
											if (FVector::Distance(commander->SiegeCoverArray[i], subAi.Key->GetActorLocation()) < 200)
											{
												arraysame = true;
											}
										}
										else
										{
											if (FVector::Distance(commander->SiegeCoverArray[i], subAi.Key->GetActorLocation()) < 50)
											{
												arraysame = true;
											}
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
		
	}
	return EBTNodeResult::Succeeded;
}
