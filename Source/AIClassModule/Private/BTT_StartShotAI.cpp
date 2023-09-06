// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_StartShotAI.h"
#include "AIWeaponComponent.h"

UBTT_StartShotAI::UBTT_StartShotAI()
{
	NodeName = TEXT("StartShotAI");
}

EBTNodeResult::Type UBTT_StartShotAI::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UAIWeaponComponent* AIWeapon = OwnerComp.GetOwner()->FindComponentByClass<UAIWeaponComponent>();
	AIWeapon->StartFire();

	return EBTNodeResult::Succeeded;
}
