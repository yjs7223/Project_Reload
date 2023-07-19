// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverAnimInstance.h"
#include "WeaponComponent.h"
#include "GameFramework/Character.h"
#include "CoverComponent.h"
#include "BaseInputComponent.h"
//#include "WeaponMeshComponent.h"


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
	
	if (mIsCover && !mIsPeeking && !mIsCoverShooting && (m_Input->getInput()->IsAiming || m_Input->getInput()->IsFire)) {
		mSpinRotater = FRotator(0.0, 180.0, 0.0);
	}
	else if (mWeapon && mIsReload && !mIsPeeking) {
		mSpinRotater = FRotator(0.0, 90.0 * (mIsFaceRight ? 1.0 : 3.0), 0.0);
	}
	else {
		mSpinRotater = FRotator(0.0, 0.0, 0.0);
	}

	mSpinRotater = FRotator(0.0, 0.0, 0.0);
	SetHandleing(DeltaSeconds);
}

void UCoverAnimInstance::SetHandleing(float DeltaTime)
{
	if (mWeaponMesh == nullptr) return;
	//if (mIsAiming == true) return;
	if (mIsCover == false || mIsFaceRight) {
		mWeaponMesh->AttachToComponent(
			dynamic_cast<ACharacter*>(TryGetPawnOwner())->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, mRightHandName);
		mWeaponMesh->SetRelativeRotation(FRotator(0, 0, 0));
		mWeaponMesh->SetRelativeLocation(FVector(6, 0, 0));
	}
	else {
		mWeaponMesh->AttachToComponent(
			dynamic_cast<ACharacter*>(TryGetPawnOwner())->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, mLeftHandName);
		mWeaponMesh->SetRelativeRotation(FRotator(0, 180, 180));
		mWeaponMesh->SetRelativeLocation(FVector(-6, 0, 0));
	}
}
