// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_PlayerInCoverChk.h"
#include "AICharacter.h"
#include "AICommander.h"
#include "CoverComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_PlayerInCoverChk::UBTS_PlayerInCoverChk()
{
	NodeName = TEXT("PlayerInCoverChk");
}

void UBTS_PlayerInCoverChk::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AIController = nullptr;
	AIController = Cast<AAICommander>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		if (AIController->GetBlackboardComponent())
		{
			if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->FindComponentByClass<UCoverComponent>()->IsCover())
			{
				AIController->GetBlackboardComponent()->SetValueAsBool("Target_Cover", true);
			}
			else if (!UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->FindComponentByClass<UCoverComponent>()->IsCover())
			{
				AIController->GetBlackboardComponent()->SetValueAsBool("Target_Cover", false);
			}
		}

	}
}
