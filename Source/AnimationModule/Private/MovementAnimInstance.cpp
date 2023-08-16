// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementAnimInstance.h"
#include "GameFramework/Character.h"
#include "PlayerMoveComponent.h"
#include "WeaponComponent.h"
#include "KismetAnimationLibrary.h"
#include "BaseInputComponent.h"
#include "BaseCharacterMovementComponent.h"

#include "Kismet/KismetSystemLibrary.h"

bool FMovementAimationTable::IsVaild()
{
	static FMovementAimationTable emptyAnimation = FMovementAimationTable();
	return 0 != memcmp(this, &emptyAnimation, sizeof(FMovementAimationTable));
}


UMovementAnimInstance::UMovementAnimInstance()
{
}

void UMovementAnimInstance::NativeBeginPlay()
{
	owner = Cast<ACharacter>(TryGetPawnOwner());
	m_Input = owner->FindComponentByClass<UBaseInputComponent>();
	m_Movement = owner->FindComponentByClass<UBaseCharacterMovementComponent>();
	AnimationSetting();
}

void UMovementAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (owner) {
		FVector tempVelocity = owner->GetVelocity();
		mMoveSpeed = tempVelocity.Length();
		mDirection = UKismetAnimationLibrary::CalculateDirection(tempVelocity, owner->GetActorRotation());

		mIsMoving = mMoveSpeed > 0;
		mIsCrouching = owner->bIsCrouched;
	}
	if (m_Movement) {
		mIsRuning = m_Movement->isRuning();
	}
}

void UMovementAnimInstance::AnimationSetting()
{
	UWeaponComponent* mWeapon = owner->FindComponentByClass<UWeaponComponent>();
	if (!m_AnimationTable) return;
	if (!mWeapon) return;

	static const UEnum* WeaponTypeEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);

	FMovementAimationTable* findanimation = m_AnimationTable->FindRow<FMovementAimationTable>(FName((WeaponTypeEnum->GetDisplayNameTextByValue((int)mWeapon->weapontype).ToString())), TEXT(""));
	if (findanimation && findanimation->IsVaild()) {
		m_CurrentAnimation = *findanimation;
	}
}


;
