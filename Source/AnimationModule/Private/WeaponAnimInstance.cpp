// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAnimInstance.h"
#include "WeaponComponent.h"
#include "playerWeaponComponent.h"
#include "PlayerInputComponent.h"
#include "BaseCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "BaseInputComponent.h"
#include "CoverComponent.h"
#include "Pakurable.h"


bool FWeaponAnimationTable::IsVaild()
{
	static FWeaponAnimationTable emptyAnimation = FWeaponAnimationTable();
	return 0 != memcmp(this, &emptyAnimation, sizeof(FWeaponAnimationTable));
}

UWeaponAnimInstance::UWeaponAnimInstance()
{

}

void UWeaponAnimInstance::NativeBeginPlay()
{
	m_Owner = Cast<ACharacter>(TryGetPawnOwner());
	m_Input = m_Owner->FindComponentByClass<UBaseInputComponent>();
	m_Movement = m_Owner->FindComponentByClass<UBaseCharacterMovementComponent>();
	mWeapon = m_Owner->FindComponentByClass<UWeaponComponent>();
	m_Cover = m_Owner->FindComponentByClass<UCoverComponent>();

	mWeapon->shootingAnimation.AddLambda(
		[this]() {
			Montage_Play(m_CurrentAnimation.Shooting);
		}
	);

	UPlayerInputComponent* m_PlayerInput = Cast<UPlayerInputComponent>(m_Input);
	if (m_PlayerInput) {
		m_PlayerInput->OnChangedWeapon.AddLambda([this]() {
			Montage_Play(m_CurrentAnimation.UnEquipWeapon);
			});
	}
	TArray<UActorComponent*> pakurArr = m_Owner->GetComponentsByInterface(UPakurable::StaticClass());
	if (pakurArr.Num() == 1) {
		m_PakurComp = pakurArr[0];
	}

	AnimationSetting();
}

void UWeaponAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!(m_Input)) return;
	if (!(mWeapon)) return;
	if (!m_Movement) return;
	if (!m_Cover) return;

	mIsAiming = m_Input->getInput()->IsAiming && !mWeapon->IsWeaponBlocking();
	mIsFire = m_Input->getInput()->IsFire && !mWeapon->IsWeaponBlocking();
	mIsReload = m_Input->getInput()->IsReload;

	mAimYaw = mWeapon->getAimYaw();
	mAimPitch = mWeapon->getAimPitch();
	m_CanShooting = mWeapon->m_CanShooting;
	
	m_UpperMirror = !m_Cover->IsFaceRight();

	mIsRuning = m_Movement->isRuning();

	m_IsPakuru = false;

	if (m_PakurComp && m_PakurComp->GetClass()) {
		m_IsPakuru = IPakurable::Execute_IsRolling(m_PakurComp);
	}
	m_IsUsingWeapon = (mIsAiming || mIsFire) && !m_IsPakuru && !mIsRuning;

	m_UseUnderBody = m_Cover->IsCover() || (m_IsUsingWeapon && m_Movement->Velocity.SizeSquared() <= 100.0 && !m_Owner->bIsCrouched);
}
	
void UWeaponAnimInstance::AnimationSetting()
{
	if (!m_AnimationTable) return;
	if (!mWeapon) return;

	static const UEnum* WeaponTypeEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);

	FWeaponAnimationTable* findanimation = m_AnimationTable->FindRow<FWeaponAnimationTable>(FName((WeaponTypeEnum->GetDisplayNameTextByValue((int)mWeapon->weapontype).ToString())), TEXT(""));
	if (findanimation && findanimation->IsVaild()) {
		m_CurrentAnimation = *findanimation;
	}
}

void UWeaponAnimInstance::PlayEquipMontage()
{
	Montage_Play(m_CurrentAnimation.EquipWeapon);
}
