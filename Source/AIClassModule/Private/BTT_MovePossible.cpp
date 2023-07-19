// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_MovePossible.h"
#include "AI_Controller.h"
#include "AICommander.h"
#include "AICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_MovePossible::UBTT_MovePossible()
{
	NodeName = TEXT("MovePossible");
}

EBTNodeResult::Type UBTT_MovePossible::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;
	
	AIController = nullptr;
	ACharacter = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ACharacter)
	{
		AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ACharacter)->GetController());
	}
	if (AIController)
	{
		if (AIController->BlackboardComponent)
		{
			BlackboardComponent = AIController->BlackboardComponent;
			BlackboardComponent->SetValueAsFloat("AI_Dist", 
				FVector::Dist(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation()));
			if (BlackboardComponent->GetValueAsFloat("Sup_TotalPoint") > 30.0f)
			{
				return EBTNodeResult::Failed;
			}
			else
			{
				dist = BlackboardComponent->GetValueAsFloat("AI_Dist");
				for (auto ai : Cast<AAICommander>(commander)->List_Division)
				{
					if (ai.Key != OwnerComp.GetAIOwner()->GetPawn())
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
								if (dist < BlackboardComponent->GetValueAsFloat("AI_Dist"))
								{
									return EBTNodeResult::Failed;
								}
							}
						}
					}
				}
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
