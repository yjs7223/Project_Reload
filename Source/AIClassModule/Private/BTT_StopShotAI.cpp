// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_StopShotAI.h"
#include "AIWeaponComponent.h"

UBTT_StopShotAI::UBTT_StopShotAI()
{
	NodeName = TEXT("StopShotAI");
}

EBTNodeResult::Type UBTT_StopShotAI::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UAIWeaponComponent* AIWeapon = OwnerComp.GetOwner()->FindComponentByClass<UAIWeaponComponent>();
	AIWeapon->StopFire();

	return EBTNodeResult::Succeeded;
}
