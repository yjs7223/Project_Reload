// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SiegeCoverSelection.h"
#include "AI_Controller.h"
#include "AICommander.h"
#include "AICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_SiegeCoverSelection::UBTT_SiegeCoverSelection()
{
	maplistnum = 0;
}

EBTNodeResult::Type UBTT_SiegeCoverSelection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	maplistnum = Cast<AAICommander>(OwnerComp.GetAIOwner())->SiegeCoverArray.Num();
	enemyActivenum = 0;
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
				if (enemyActivenum <= maplistnum)
				{
					if (BlackboardComponent->GetValueAsEnum("Combat") == 4)
					{
						enemyActivenum++;
					}
				}
				
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
