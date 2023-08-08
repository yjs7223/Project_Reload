// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAnimInstance.h"
#include "WeaponComponent.h"

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
	m_Input = dynamic_cast<UBaseInputComponent*>(TryGetPawnOwner()->FindComponentByClass<UBaseInputComponent>());

	ACharacter* owner = Cast<ACharacter>(TryGetPawnOwner());
	mWeapon = owner->FindComponentByClass<UWeaponComponent>();
	mPlayerMove = owner->FindComponentByClass<UPlayerMoveComponent>();

	AnimationSetting();
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
