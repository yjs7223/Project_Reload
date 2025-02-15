// Fill out your copyright notice in the Description page of Project Settings.
#include "StatsAnimInstance.h"
#include "GameFramework/Character.h"
#include "WeaponComponent.h"
#include "StatComponent.h"
#include "Pakurable.h"
#include "BaseInputComponent.h"


bool FStatAnimationTable::IsVaild()
{
	static FStatAnimationTable emptyAnimation = FStatAnimationTable();
	return 0 != memcmp(this, &emptyAnimation, sizeof(FStatAnimationTable));
}

UStatsAnimInstance::UStatsAnimInstance()
{
	m_IsStun = false;
	m_IsKnockback = false;
}

void UStatsAnimInstance::NativeBeginPlay()
{
	owner = Cast<ACharacter>(TryGetPawnOwner());
	mStats = owner->FindComponentByClass<UStatComponent>();
	m_Input = owner->FindComponentByClass<UBaseInputComponent>();
	AnimationSetting();


	mStats->Knockback.AddLambda([this](FVector directinal, bool bDie) {
		FVector vector = owner->GetActorLocation() - directinal;
		float dotvlaue = vector.Dot(owner->GetActorForwardVector());
		UAnimMontage* currmontage;
		if (dotvlaue > 0.5f) {
			currmontage = bDie ? m_CurrentAnimation.KnockBack_Front_Die : m_CurrentAnimation.KnockBack_Front;
		}
		else if (dotvlaue > -0.5f) {
			if (vector.Dot(owner->GetActorRightVector())) {
				currmontage = bDie ? m_CurrentAnimation.KnockBack_Right_Die : m_CurrentAnimation.KnockBack_Right;
			}
			else {
				currmontage = bDie ? m_CurrentAnimation.KnockBack_Left_Die : m_CurrentAnimation.KnockBack_Left;
			}
		}
		else {
			currmontage = bDie ? m_CurrentAnimation.KnockBack_Back_Die : m_CurrentAnimation.KnockBack_Back;
		}
		FOnMontageEnded MontageEndDelegate = FOnMontageEnded::CreateUObject(this, &UStatsAnimInstance::KnockBackEnd);
		UAnimInstance* animinstance = owner->GetMesh()->GetAnimInstance();
		animinstance->Montage_Play(currmontage);
		mStats->SetKnockback(true);
		if (m_PakurComp) {
			animinstance->Montage_SetEndDelegate(MontageEndDelegate, currmontage);
			if (m_PakurComp && m_PakurComp->GetClass()->ImplementsInterface(UPakurable::StaticClass())) {
				IPakurable::Execute_SetCanRolling(m_PakurComp, false);
			}
		}
		});

	TArray<UActorComponent*> pakurArr = owner->GetComponentsByInterface(UPakurable::StaticClass());
	if (pakurArr.Num() == 1) {
		m_PakurComp = pakurArr[0];
	}

	mStats->diePlay.AddDynamic(this, &UStatsAnimInstance::PlayDIeMontage);
	//owner->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UStatsAnimInstance::KnockBackEnd);

}

void UStatsAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!(mStats)) return;

	mIsDie = mStats->bDie;
	mIsHit = mStats->bAttacked;
	m_HitX = mStats->hitNormal.X;
	m_HitY = mStats->hitNormal.Y;
	m_HitReactionScale = mStats->HitReactionScale;

	m_IsStun = mStats->IsStun();
}

void UStatsAnimInstance::AnimationSetting()
{
	UWeaponComponent* mWeapon = owner->FindComponentByClass<UWeaponComponent>();
	if (!m_AnimationTable) return;
	if (!mWeapon) return;

	static const UEnum* WeaponTypeEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);

	FStatAnimationTable* findanimation = m_AnimationTable->FindRow<FStatAnimationTable>(FName((WeaponTypeEnum->GetDisplayNameTextByValue((int)mWeapon->weapontype).ToString())), TEXT(""));
	if (findanimation && findanimation->IsVaild()) {
		m_CurrentAnimation = *findanimation;
	}
}

void UStatsAnimInstance::KnockBackEnd(UAnimMontage* Montage, bool bInterrupted)
{
	mStats->SetKnockback(false);
	if (Montage->IsValidSlot("AdditiveHitReact")) {
		if (m_PakurComp && m_PakurComp->GetClass()->ImplementsInterface(UPakurable::StaticClass())) {
			IPakurable::Execute_SetCanRolling(m_PakurComp, true);
		}
	}
}

void UStatsAnimInstance::PlayDIeMontage()
{
	UAnimInstance* animinstance = owner->GetMesh()->GetAnimInstance();
	if (!animinstance->Montage_IsPlaying(DieMontage)) {
		animinstance->Montage_Play(DieMontage);
	}
}
