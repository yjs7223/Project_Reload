// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_CoverPossiblePoint.h"
#include "AI_Controller.h"
#include "AICommander.h"
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

					if (*Cast<AAICommander>(OwnerComp.GetAIOwner())->List_CoverPoint.Find(ai.Value) == FVector(0, 0, 0))
					{
						mindis = NULL;
						for (auto coverenemy : Cast<AAICommander>(OwnerComp.GetAIOwner())->CoverEnemyArray)
						{
							if (mindis == NULL)
							{
								mindis = FVector::Distance(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), coverenemy);
								mindislocation = coverenemy;
								
							}
							else
							{
								if (mindis > FVector::Distance(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), coverenemy))
								{
									mindis = FVector::Distance(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), coverenemy);
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
							if (*Cast<AAICommander>(commander)->List_CoverPoint.Find(ai.Value) == coverenemy)
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
									mindis = FVector::Distance(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), coverenemy);
									mindislocation = coverenemy;
								}
								else
								{
									if (mindis > FVector::Distance(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), coverenemy))
									{
										mindis = FVector::Distance(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), coverenemy);
										mindislocation = coverenemy;
									}
								}
							}
						}
					}
					same = false;
					for (auto coverpoint : Cast<AAICommander>(OwnerComp.GetAIOwner())->List_CoverPoint)
					{
						if (coverpoint.Key != ai.Value)
						{
							if (mindislocation == coverpoint.Value)
							{
								same = true;
							}
						}
					}
					if (!same)
					{
						BlackboardComponent->SetValueAsVector("AI_MoveLocation", mindislocation);
					}
					else if (same)
					{
						arraysame = false;
						int num = 1;
						for (int i = 0; i < num; i++)
						{
							for (auto coverpoint : Cast<AAICommander>(OwnerComp.GetAIOwner())->List_CoverPoint)
							{
								if (coverpoint.Key != ai.Value)
								{
									if (Cast<AAICommander>(OwnerComp.GetAIOwner())->CoverEnemyArray[i] == coverpoint.Value)
									{
										arraysame = true;
									}
								}
							}
							if (arraysame)
							{
								num++;
								if (num >= Cast<AAICommander>(OwnerComp.GetAIOwner())->CoverEnemyArray.Num())
								{
									BlackboardComponent->SetValueAsBool("OrderWait", false);
									return EBTNodeResult::Succeeded;
								}
							}
							else if (!arraysame)
							{
								BlackboardComponent->SetValueAsVector("AI_MoveLocation", Cast<AAICommander>(OwnerComp.GetAIOwner())->CoverEnemyArray[i]);
							}
						}
					}

					BlackboardComponent->SetValueAsBool("OrderWait", false);
				}
			}
		}
		
	}
	
	return EBTNodeResult::Succeeded;
}
