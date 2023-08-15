// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_CoverPossiblePoint.h"
#include "AICommander.h"
#include "AI_Controller.h"
#include "AICharacter.h"
#include "SubEncounterSpace.h"
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
	if (!OwnerComp.GetAIOwner())
	{
		return EBTNodeResult::Succeeded;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, mindislocation.ToString());
	for (auto enemy : commander->List_Division)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, OwnerComp.GetAIOwner()->GetPawn()->GetName());
		//ACharacter = Cast<AAICharacter>();
		AIController = Cast<AAI_Controller>(Cast<AAICharacter>(enemy.Key)->GetController());
		if (!AIController)
		{
			continue;
		}
		UBlackboardComponent* blackbordComponent = AIController->GetBlackboardComponent();
		if (!blackbordComponent || !blackbordComponent->GetValueAsBool("OrderWait"))
		{
			continue;
		}
		if (commander->CoverEnemyArray.IsEmpty())
		{
			blackbordComponent->SetValueAsBool("OrderWait", false);
			return EBTNodeResult::Succeeded;
		}

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
				for (auto subAi : commander->List_Division)
				{
					if (subAi.Key != enemy.Key)
					{
						if (!Cast<AAI_Controller>(Cast<AAICharacter>(subAi.Key)->GetController()))
						{
							continue;
						}
						AAI_Controller* sub_aic = Cast<AAI_Controller>(Cast<AAICharacter>(subAi.Key)->GetController());
						if (FVector::Distance(sub_aic->GetBlackboardComponent()->GetValueAsVector("AI_MoveLocation"), coverenemy) < 200)
						{
							B_distance = true;
						}
						if (FVector::Distance(coverenemy, subAi.Key->GetActorLocation()) < 200)
						{
							B_distance = true;
						}
					}
					else
					{
						if (FVector::Distance(coverenemy, subAi.Key->GetActorLocation()) < 50)
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
				if (FVector::Distance(enemy.Key->GetActorLocation(), coverenemy) < 200)
				{
					coverpossible = true;
					blackbordComponent->SetValueAsVector("AI_CoverLocation", coverenemy);
					blackbordComponent->SetValueAsBool("OrderWait", false);
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
						if (FVector::Distance(coverenemy, coverlist.Value) < 200)
						{
							B_distance = true;
						}
					}
					for (auto subAi : commander->List_Division)
					{
						if (subAi.Key != enemy.Key)
						{
							if (!Cast<AAI_Controller>(Cast<AAICharacter>(subAi.Key)->GetController()))
							{
								continue;
							}
							AAI_Controller* sub_aic = Cast<AAI_Controller>(Cast<AAICharacter>(subAi.Key)->GetController());
							if (FVector::Distance(sub_aic->GetBlackboardComponent()->GetValueAsVector("AI_MoveLocation"), coverenemy) < 200)
							{
								B_distance = true;
							}
							if (FVector::Distance(coverenemy, subAi.Key->GetActorLocation()) < 200)
							{
								B_distance = true;
							}
						}
						else
						{
							if (FVector::Distance(coverenemy, subAi.Key->GetActorLocation()) < 50)
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
		}
		if (mindislocation == FVector::ZeroVector)
		{
			blackbordComponent->SetValueAsBool("OrderWait", false);
			return EBTNodeResult::Succeeded;
		}
		blackbordComponent->SetValueAsVector("AI_CoverLocation", mindislocation);
		commander->List_CoverPoint.Add(enemy.Value, mindislocation);
		
		blackbordComponent->SetValueAsBool("OrderWait", false);
	}

	return EBTNodeResult::Succeeded;
}
