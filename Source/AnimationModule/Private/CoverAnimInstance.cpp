// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverAnimInstance.h"
#include "WeaponComponent.h"
#include "GameFramework/Character.h"
#include "CoverComponent.h"
#include "BaseInputComponent.h"
#include <Kismet/KismetSystemLibrary.h>

bool FCoverAnimationTable::IsVaild()
{
	static FCoverAnimationTable emptyAnimation = FCoverAnimationTable();
	return 0 != memcmp(this, &emptyAnimation, sizeof(FCoverAnimationTable));
}

UCoverAnimInstance::UCoverAnimInstance()
{
	mRightHandName = TEXT("hand_r_Socket");
	mLeftHandName = TEXT("hand_l_Socket");
}

void UCoverAnimInstance::NativeBeginPlay()
{
	ACharacter* owner = Cast<ACharacter>(TryGetPawnOwner());

	m_Input = dynamic_cast<UBaseInputComponent*>(TryGetPawnOwner()->FindComponentByClass<UBaseInputComponent>());
	mCover = owner->FindComponentByClass<UCoverComponent>();
	mWeapon = owner->FindComponentByClass<UWeaponComponent>();
	if (mWeapon) {
		mWeaponMesh = mWeapon->WeaponMesh;
	}
}

void UCoverAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (m_Input) {
		FInputData* data = m_Input->getInput();
		mIsReload = data->IsReload;
		mIsFire = data->IsFire;
		mIsAiming = data->IsAiming;
	}
	if (mWeapon) {
		mAimYaw = mWeapon->getAimYaw();
		mAimPitch = mWeapon->getAimPitch();
	}
	if (mCover) {
		mIsCover = mCover->IsCover();
		mIsFaceRight = mCover->FaceRight() >= 0;
		mIsCornering = mCover->IsCornering();
		mPeekingState = mCover->getPeekingState();
		mCoverSootingState = mCover->getCoverSootingState();
		mIsConerWait = mCover->IsTurnWait();
		if (mPeekingState != EPeekingState::None) {
			mLastPeekingState = mPeekingState;
		}
		

		mIsPeeking = mLastPeekingState != EPeekingState::None;
		mIsCoverShooting = mCoverSootingState != ECoverShootingState::None;
	}

	if (ACharacter* charcter = dynamic_cast<ACharacter*>(TryGetPawnOwner())) {
		mIsCrouching = charcter->bIsCrouched;
		mIsMoving = charcter->GetVelocity().Length() > 0 || mIsCornering;
	}
}

