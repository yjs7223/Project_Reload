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
	if (OwnerComp.GetAIOwner())
	{
		for (auto enemy : Cast<AAICommander>(OwnerComp.GetAIOwner())->List_Division)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, OwnerComp.GetAIOwner()->GetPawn()->GetName());
			AIController = nullptr;
			//ACharacter = Cast<AAICharacter>();
	
			AIController = Cast<AAI_Controller>(Cast<AAICharacter>(enemy.Key)->GetController());

			if (AIController)
			{
				if (AIController->GetBlackboardComponent())
				{

					if (AIController->GetBlackboardComponent()->GetValueAsBool("OrderWait"))
					{

						if (*Cast<AAICommander>(OwnerComp.GetAIOwner())->List_CoverPoint.Find(enemy.Value) == FVector(0, 0, 0))
						{
							mindis = NULL;
							for (auto coverenemy : Cast<AAICommander>(OwnerComp.GetAIOwner())->CoverEnemyArray)
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
							for (auto coverenemy : Cast<AAICommander>(OwnerComp.GetAIOwner())->CoverEnemyArray)
							{
								if (*Cast<AAICommander>(commander)->List_CoverPoint.Find(enemy.Value) == coverenemy)
								{
									coverpossible = true;
								}
							}
							if (!coverpossible)
							{
								for (auto coverenemy : Cast<AAICommander>(OwnerComp.GetAIOwner())->CoverEnemyArray)
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
						for (auto coverpoint : Cast<AAICommander>(OwnerComp.GetAIOwner())->List_CoverPoint)
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
							AIController->GetBlackboardComponent()->SetValueAsVector("AI_MoveLocation", mindislocation);
						}
						else if (same)
						{
							arraysame = false;
							int num = 1;
							for (int i = 0; i < num; i++)
							{
								for (auto coverpoint : Cast<AAICommander>(OwnerComp.GetAIOwner())->List_CoverPoint)
								{
									if (coverpoint.Key != enemy.Value)
									{
										if (!Cast<AAICommander>(OwnerComp.GetAIOwner())->CoverEnemyArray.IsEmpty())
										{
											if (Cast<AAICommander>(OwnerComp.GetAIOwner())->CoverEnemyArray[i] == coverpoint.Value)
											{
												arraysame = true;
											}
										}

									}
								}
								if (arraysame)
								{
									num++;
									if (num >= Cast<AAICommander>(OwnerComp.GetAIOwner())->CoverEnemyArray.Num())
									{
										AIController->GetBlackboardComponent()->SetValueAsBool("OrderWait", false);
										return EBTNodeResult::Succeeded;
									}
								}
								else if (!arraysame)
								{
									if (num >= Cast<AAICommander>(OwnerComp.GetAIOwner())->CoverEnemyArray.Num())
									{
										AIController->GetBlackboardComponent()->SetValueAsBool("OrderWait", false);
										return EBTNodeResult::Succeeded;
									}
									AIController->GetBlackboardComponent()->SetValueAsVector("AI_MoveLocation", Cast<AAICommander>(OwnerComp.GetAIOwner())->CoverEnemyArray[i]);
								}
							}
						}

						AIController->GetBlackboardComponent()->SetValueAsBool("OrderWait", false);
					}
				}
			}

		}
	}

	return EBTNodeResult::Succeeded;
}
