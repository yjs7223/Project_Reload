// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_CoverPossiblePoint.h"
#include "AICommander.h"
#include "AI_Controller.h"
#include "AICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_CoverPossiblePoint::UBTT_CoverPossiblePoint()
{
	NodeName = TEXT("CoverPossiblePoint");
	coverpossible = false;
	same = false;
	arraysame = false;
}

EBTNodeResult::Type UBTT_CoverPossiblePoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!commander)
	{
		commander = Cast<AAICommander>(OwnerComp.GetAIOwner());
	}
	if (OwnerComp.GetAIOwner())
	{
		for (auto enemy : commander->List_Division)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, OwnerComp.GetAIOwner()->GetPawn()->GetName());
			AIController = nullptr;
			//ACharacter = Cast<AAICharacter>();
	
			AIController = Cast<AAI_Controller>(Cast<AAICharacter>(enemy.Key)->GetController());

			if (AIController)
			{
				if (AIController->GetBlackboardComponent())
				{

					if (AIController->GetBlackboardComponent()->GetValueAsBool("OrderWait"))
					{

						if (*commander->List_CoverPoint.Find(enemy.Value) == FVector(0, 0, 0))
						{
							mindis = NULL;
							for (auto coverenemy : commander->CoverEnemyArray)
							{
								if (mindis == NULL)
								{
									mindis = FVector::Distance(enemy.Key->GetActorLocation(), coverenemy);
									mindislocation = coverenemy;

								}
								else
								{
									if (mindis > FVector::Distance(enemy.Key->GetActorLocation(), coverenemy))
									{
										mindis = FVector::Distance(enemy.Key->GetActorLocation(), coverenemy);
										mindislocation = coverenemy;
									}
								}
							}
						}
						else
						{
							coverpossible = false;
							for (auto coverenemy : commander->CoverEnemyArray)
							{
								if (*commander->List_CoverPoint.Find(enemy.Value) == coverenemy)
								{
									coverpossible = true;
								}
							}
							if (!coverpossible)
							{
								for (auto coverenemy : commander->CoverEnemyArray)
								{
									if (mindis == NULL)
									{
										mindis = FVector::Distance(enemy.Key->GetActorLocation(), coverenemy);
										mindislocation = coverenemy;
									}
									else
									{
										if (mindis > FVector::Distance(enemy.Key->GetActorLocation(), coverenemy))
										{
											mindis = FVector::Distance(enemy.Key->GetActorLocation(), coverenemy);
											mindislocation = coverenemy;
										}
									}
								}
							}
						}
						same = false;
						for (auto coverpoint : commander->List_CoverPoint)
						{
							if (coverpoint.Key != enemy.Value)
							{
								if (mindislocation == coverpoint.Value)
								{
									same = true;
								}
							}
						}
						if (!same)
						{
							AIController->GetBlackboardComponent()->SetValueAsVector("AI_CoverLocation", mindislocation);
							commander->List_CoverPoint.Add(enemy.Value, mindislocation);
							AIController->GetBlackboardComponent()->SetValueAsBool("OrderWait", false);
						}
						else if (same)
						{
							arraysame = false;
							int num = 1;
							for (int i = 0; i < num; i++)
							{
								for (auto coverpoint : commander->List_CoverPoint)
								{
									if (coverpoint.Key != enemy.Value)
									{
										if (!commander->CoverEnemyArray.IsEmpty())
										{
											if (commander->CoverEnemyArray[i] == coverpoint.Value)
											{
												arraysame = true;
											}
										}

									}
								}
								if (arraysame)
								{
									num++;
									if (num >= commander->CoverEnemyArray.Num())
									{
										AIController->GetBlackboardComponent()->SetValueAsBool("OrderWait", false);
										return EBTNodeResult::Succeeded;
									}
								}
								else if (!arraysame)
								{
									if (num >= commander->CoverEnemyArray.Num())
									{
										AIController->GetBlackboardComponent()->SetValueAsBool("OrderWait", false);
										return EBTNodeResult::Succeeded;
									}
									
									AIController->GetBlackboardComponent()->SetValueAsVector("AI_CoverLocation", commander->CoverEnemyArray[i]);
									commander->List_CoverPoint.Add(enemy.Value, commander->CoverEnemyArray[i]);
									AIController->GetBlackboardComponent()->SetValueAsBool("OrderWait", false);
								}
							}
						}
						FVector test  =   AIController->GetBlackboardComponent()->GetValueAsVector("AI_CoverLocation");
						
					}
				}
			}

		}
	}

	return EBTNodeResult::Succeeded;
}
