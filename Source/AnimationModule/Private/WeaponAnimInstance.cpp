// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAnimInstance.h"
#include "WeaponComponent.h"
#include "BaseCharacterMovementComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "PlayerMoveComponent.h"
#include "BaseInputComponent.h"


UWeaponAnimInstance::UWeaponAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/ATH/Animation/DT_WeaponAnimation.DT_WeaponAnimation'"));
	if (DataTable.Succeeded())
	{
		m_AnimationTable = DataTable.Object;
	}
}

void UWeaponAnimInstance::NativeBeginPlay()
{
	ACharacter* owner = Cast<ACharacter>(TryGetPawnOwner());
	m_Input = owner->FindComponentByClass<UBaseInputComponent>();

	mWeapon = owner->FindComponentByClass<UWeaponComponent>(); 
	mWeapon->shootingAnimation.BindLambda([this]() {
		Montage_Play(m_CurrentAnimation.shooting);
		}
	);
	m_Movement = owner->FindComponentByClass<UBaseCharacterMovementComponent>();
	AnimationSetting();
}

void UWeaponAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (m_Input) {
		mIsAiming = m_Input->getInput()->IsAiming;
		mIsFire = m_Input->getInput()->IsFire;
		mIsReload = m_Input->getInput()->IsReload;
	}
	if (mWeapon) {
		mAimYaw = mWeapon->getAimYaw();
		mAimPitch = mWeapon->getAimPitch();
	}
	if (m_Movement) {
		mIsRuning = m_Movement->isRuning();
	}
	if (Montage_IsPlaying(m_CurrentAnimation.shooting)) {
		int a = 5;
	}
}
	
void UWeaponAnimInstance::AnimationSetting()
{
	if (!m_AnimationTable) return;
	if (!mWeapon) return;

	static const UEnum* WeaponTypeEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);

	FWeaponAnimation* findanimation = m_AnimationTable->FindRow<FWeaponAnimation>(*WeaponTypeEnum->GetNameStringByValue((int)mWeapon->weapontype), TEXT(""));
	
	if (findanimation && findanimation->IsVaild()) {
		m_CurrentAnimation = *findanimation;
	}
	else {

	}
}

void UWeaponAnimInstance::PlayShootingAnimation()
{
	Montage_Play(m_CurrentAnimation.shooting);
}
