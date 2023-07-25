// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_PlayerStateChk.h"
#include "AICharacter.h"
#include "AICommander.h"
#include "BaseInputComponent.h"
#include "CoverComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

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
	//AIController = nullptr;
	if (!AIController)
	{
		AIController = Cast<AAICommander>(OwnerComp.GetAIOwner());
	}
	
	if (AIController)
	{
		if (AIController->GetBlackboardComponent())
		{
			UBaseInputComponent* Inputcomp = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->FindComponentByClass<UBaseInputComponent>();
			UCoverComponent* Covercomp = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->FindComponentByClass<UCoverComponent>();

			FInputData data;
			memset(&data, 0, sizeof(FInputData));
			if (Inputcomp->getInput()->IsFire || Inputcomp->getInput()->IsAiming)
			{
				AIController->GetBlackboardComponent()->SetValueAsEnum("Target_State", 1);
			}
			else if (Covercomp->IsCover())
			{
				AIController->GetBlackboardComponent()->SetValueAsEnum("Target_State", 2);
			}
			else if (Inputcomp->getInput()->IsRuning)
			{
				AIController->GetBlackboardComponent()->SetValueAsEnum("Target_State", 3);
			}
			else if (!memcmp(Inputcomp->getInput(), &data, sizeof(FInputData))) {
				AIController->GetBlackboardComponent()->SetValueAsEnum("Target_State", 0);
			}
			
		}
	}

}
