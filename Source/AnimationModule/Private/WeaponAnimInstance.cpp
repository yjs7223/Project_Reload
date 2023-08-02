// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAnimInstance.h"
#include "WeaponComponent.h"

#include "WeaponAnimDataAsset.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "PlayerMoveComponent.h"
#include "BaseInputComponent.h"


UWeaponAnimInstance::UWeaponAnimInstance()
{
}

void UWeaponAnimInstance::NativeBeginPlay()
{
	m_Input = dynamic_cast<UBaseInputComponent*>(TryGetPawnOwner()->FindComponentByClass<UBaseInputComponent>());

	ACharacter* owner = Cast<ACharacter>(TryGetPawnOwner());
	mWeapon = owner->FindComponentByClass<UWeaponComponent>();
	mPlayerMove = owner->FindComponentByClass<UPlayerMoveComponent>();
}

void UWeaponAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (m_Input) {
		mIsAiming = m_Input->getInput()->IsAiming;
		mIsFire = m_Input->getInput()->IsFire;
		mIsReload = m_Input->getInput()->IsReload;
		mIsRuning = m_Input->getInput()->IsRuning;
	}
	if (mWeapon) {
		mAimYaw = mWeapon->getAimYaw();
		mAimPitch = mWeapon->getAimPitch();
	}
}
