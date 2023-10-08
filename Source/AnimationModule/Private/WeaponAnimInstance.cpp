// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAnimInstance.h"
#include "WeaponComponent.h"
#include "playerWeaponComponent.h"
#include "PlayerInputComponent.h"
#include "BaseCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "BaseInputComponent.h"


bool FWeaponAnimationTable::IsVaild()
{
	static FWeaponAnimationTable emptyAnimation = FWeaponAnimationTable();
	return 0 != memcmp(this, &emptyAnimation, sizeof(FWeaponAnimationTable));
}

UWeaponAnimInstance::UWeaponAnimInstance()
{
	/*static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/ATH/Animation/DT_WeaponAnimation.DT_WeaponAnimation'"));
	if (DataTable.Succeeded())
	{
		m_AnimationTable = DataTable.Object;
	}*/
}

void UWeaponAnimInstance::NativeBeginPlay()
{
	ACharacter* owner = Cast<ACharacter>(TryGetPawnOwner());
	m_Input = owner->FindComponentByClass<UBaseInputComponent>();

	mWeapon = owner->FindComponentByClass<UWeaponComponent>(); 
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


	m_Movement = owner->FindComponentByClass<UBaseCharacterMovementComponent>();
	AnimationSetting();
}

void UWeaponAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!(m_Input)) return;
	if (!(mWeapon)) return;

	mIsAiming = m_Input->getInput()->IsAiming && !mWeapon->IsWeaponBlocking();
	mIsFire = m_Input->getInput()->IsFire && !mWeapon->IsWeaponBlocking();
	mIsReload = m_Input->getInput()->IsReload;

	mAimYaw = mWeapon->getAimYaw();
	mAimPitch = mWeapon->getAimPitch();

	if (m_Movement) {
		mIsRuning = m_Movement->isRuning();
	}
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
