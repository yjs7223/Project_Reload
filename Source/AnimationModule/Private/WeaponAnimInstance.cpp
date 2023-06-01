// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAnimInstance.h"
#include "WeaponComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "PlayerMoveComponent.h"


UWeaponAnimInstance::UWeaponAnimInstance()
{
}

void UWeaponAnimInstance::NativeBeginPlay()
{
	ACharacter* owner = Cast<ACharacter>(TryGetPawnOwner());
	mWeapon = owner->FindComponentByClass<UWeaponComponent>();
	mPlayerMove = owner->FindComponentByClass<UPlayerMoveComponent>();
}

void UWeaponAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (mWeapon) {
		//mAimYaw = mWeapon->getAimYaw();
		//mAimPitch = mWeapon->getAimPitch();
		//mIsAiming = mWeapon->getisAiming();
		//mIsFire = mWeapon->isFire();

		//mIsReload = mWeapon->isReloading();
	}
	if (mPlayerMove) {
		//mIsRuning = mPlayerMove->IsRuning();
	}
}
