// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_PlayerStateChk.h"
#include "AICharacter.h"
#include "AICommander.h"
#include "BaseInputComponent.h"
#include "CoverComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseCharacterMovementComponent.h"

UBTS_PlayerStateChk::UBTS_PlayerStateChk()
{
	NodeName = TEXT("PlayerStateChk");
}

void UBTS_PlayerStateChk::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!player)
	{
		player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}
	//commander = nullptr;
	if (!AIController)
	{
		AIController = Cast<AAICommander>(OwnerComp.GetAIOwner());
	}
	
	if (AIController)
	{
		if (AIController->GetBlackboardComponent())
		{
			UBaseInputComponent* Inputcomp = player->FindComponentByClass<UBaseInputComponent>();
			UCoverComponent* Covercomp = player->FindComponentByClass<UCoverComponent>();
			UBaseCharacterMovementComponent* movement = player->FindComponentByClass<UBaseCharacterMovementComponent>();
			FInputData data;

			memset(&data, 0, sizeof(FInputData));

			if (Inputcomp->getInput()->IsFire || Inputcomp->getInput()->IsAiming)
			{
				AIController->GetBlackboardComponent()->SetValueAsEnum("Target_State", (uint8)ETarget_State::Attack);
			}
			else if (player->FindComponentByClass<UCoverComponent>()->IsCover())
			{
				AIController->GetBlackboardComponent()->SetValueAsEnum("Target_State", (uint8)ETarget_State::Cover);
			}
			else if (movement->isRuning())
			{
				AIController->GetBlackboardComponent()->SetValueAsEnum("Target_State", (uint8)ETarget_State::Move);
			}
			else if (!memcmp(Inputcomp->getInput(), &data, sizeof(FInputData))) {
				AIController->GetBlackboardComponent()->SetValueAsEnum("Target_State", (uint8)ETarget_State::Idle);
			}
			
		}
	}

}
