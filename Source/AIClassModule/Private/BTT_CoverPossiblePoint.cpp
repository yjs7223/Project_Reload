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
	mindislocation = FVector::ZeroVector;
	B_distance = false;
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
						beforelocation = mindislocation;
						if (*commander->List_CoverPoint.Find(enemy.Value) == FVector::ZeroVector)
						{
							mindis = NULL;
							for (auto coverenemy : commander->CoverEnemyArray)
							{
								B_distance = false;
								for (auto coverlist : commander->List_CoverPoint)
								{
									if (enemy.Value != coverlist.Key)
									{
										if (FVector::Distance(coverenemy, coverlist.Value) < 200)
										{
											B_distance = true;
										}
									}
								}
								if (!B_distance)
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
						else
						{
							mindis = NULL;
							coverpossible = false;
							for (auto coverenemy : commander->CoverEnemyArray)
							{
								if (*commander->List_CoverPoint.Find(enemy.Value) == coverenemy)
								{
									coverpossible = true;
									AIController->GetBlackboardComponent()->SetValueAsVector("AI_CoverLocation", *commander->List_CoverPoint.Find(enemy.Value));
									AIController->GetBlackboardComponent()->SetValueAsBool("OrderWait", false);
									return EBTNodeResult::Succeeded;
								}
							}
							if (!coverpossible)
							{
								for (auto coverenemy : commander->CoverEnemyArray)
								{
									B_distance = false;
									for (auto coverlist : commander->List_CoverPoint)
									{
										if (enemy.Value != coverlist.Key)
										{
											if (FVector::Distance(coverenemy, coverlist.Value) < 200)
											{
												B_distance = true;
											}
										}
									}
									if (!B_distance)
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
							else {
								AIController->GetBlackboardComponent()->SetValueAsBool("OrderWait", false);
								return EBTNodeResult::Succeeded;
							}
						}
						if (mindislocation == FVector::ZeroVector || beforelocation == mindislocation)
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
							if (mindislocation == FVector::ZeroVector || beforelocation == mindislocation)
							{
								AIController->GetBlackboardComponent()->SetValueAsBool("OrderWait", false);
								return EBTNodeResult::Succeeded;
							}
						}
						AIController->GetBlackboardComponent()->SetValueAsVector("AI_CoverLocation", mindislocation);
						commander->List_CoverPoint.Add(enemy.Value, mindislocation);
						AIController->GetBlackboardComponent()->SetValueAsBool("OrderWait", false);				
					}
				}
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
