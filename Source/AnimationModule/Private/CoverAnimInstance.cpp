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
	AnimationSetting();

	mCover->PlayMontageStartCover.AddLambda([this]() {
		if (!m_CurrentAnimation.IsVaild()) return;
		ACharacter* owner = Cast<ACharacter>(TryGetPawnOwner());
		UAnimMontage* playMontage = nullptr;
		if (!owner->bIsCrouched) {
			if (mCover->isMustCrouch()) {
				playMontage = mCover->IsFaceRight() ? m_CurrentAnimation.StartCover_HighToLow_Right : m_CurrentAnimation.StartCover_HighToLow_Left;
			}
			else {
				playMontage = mCover->IsFaceRight() ? m_CurrentAnimation.StartCover_HighToHigh_Right : m_CurrentAnimation.StartCover_HighToHigh_Left;
			}
		}
		else {
			playMontage = mCover->IsFaceRight() ? m_CurrentAnimation.StartCover_LowToLow_Right : m_CurrentAnimation.StartCover_LowToLow_Left;
		}
		Montage_Play(playMontage);

		});
	mCover->PlayMontageEndCover.AddLambda([this]() {
		if (!m_CurrentAnimation.IsVaild()) return;
		ACharacter* owner = Cast<ACharacter>(TryGetPawnOwner());
		UAnimMontage* playMontage = nullptr;
		if (!owner->bIsCrouched) {
			playMontage = mCover->IsFaceRight() ? m_CurrentAnimation.EndCover_HighToHigh_Right : m_CurrentAnimation.EndCover_HighToHigh_Left;
		}
		else {
			playMontage = mCover->IsFaceRight() ? m_CurrentAnimation.EndCover_LowToLow_Right : m_CurrentAnimation.EndCover_LowToLow_Left;
		}
		if (playMontage) {
			Montage_Play(playMontage);

		}
		});
}

void UCoverAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!(m_Input)) return;
	if (!(mWeapon)) return;
	if (!(mCover)) return;

	FInputData* data = m_Input->getInput();
	mIsReload = data->IsReload;
	mIsFire = data->IsFire && !mWeapon->IsWeaponBlocking();
	mIsAiming = data->IsAiming && !mWeapon->IsWeaponBlocking();
	
	mAimYaw = mWeapon->getAimYaw();
	mAimPitch = mWeapon->getAimPitch();
	m_IsWeaponUse = mWeapon->m_CanShooting;

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

	mSpinRotater = (m_IsWeaponUse || mIsReload) ? FRotator(0.0, 180.0, 0.0) : FRotator(0.0, 0.0, 0.0);

	if (ACharacter* charcter = dynamic_cast<ACharacter*>(TryGetPawnOwner())) {
		mIsCrouching = charcter->bIsCrouched;
		mIsMoving = charcter->GetVelocity().Length() > 0 || mIsCornering;
	}
}


void UCoverAnimInstance::AnimationSetting()
{
	if (!m_AnimationTable) return;
	if (!mWeapon) return;

	static const UEnum* WeaponTypeEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);

	FCoverAnimationTable* findanimation = m_AnimationTable->FindRow<FCoverAnimationTable>(FName((WeaponTypeEnum->GetDisplayNameTextByValue((int)mWeapon->weapontype).ToString())), TEXT(""));
	if (findanimation && findanimation->IsVaild()) {
		m_CurrentAnimation = *findanimation;
	}
}

