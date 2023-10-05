// Fill out your copyright notice in the Description page of Project Settings.
#include "StatsAnimInstance.h"
#include "GameFramework/Character.h"
#include "WeaponComponent.h"
#include "StatComponent.h"


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
	UWeaponAnimInstance();
	mStats->Knockback.AddLambda([this](FVector directinal, bool bDie) {
		FVector vector = owner->GetActorLocation() - directinal;
		float dotvlaue = vector.Dot(owner->GetActorForwardVector());
		UAnimMontage* currmontage;
		if (dotvlaue > 0.5f) {
			currmontage = bDie ? m_CurrentAnimation.KnockBack_Front_Die : m_CurrentAnimation.KnockBack_Front;
		}
		else if (dotvlaue > -0.5f) {
			if (vector.Dot(owner->GetActorRightVector())) {
				currmontage = bDie ? m_CurrentAnimation.KnockBack_Left_Die : m_CurrentAnimation.KnockBack_Left;
			}
			else {
				currmontage = bDie ? m_CurrentAnimation.KnockBack_Right_Die : m_CurrentAnimation.KnockBack_Right;
			}
		}
		else {
			currmontage = bDie ? m_CurrentAnimation.KnockBack_Back_Die : m_CurrentAnimation.KnockBack_Back;
		}
		owner->GetMesh()->GetAnimInstance()->Montage_Play(currmontage);
		});
}

void UStatsAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (mStats) {
		mIsDie = mStats->bDie;
		mIsHit = mStats->bAttacked;
		m_HitX = mStats->hitNormal.X;
		m_HitY = mStats->hitNormal.Y;
		m_HitReactionScale = mStats->HitReactionScale;

		m_IsStun = mStats->IsStun();
	}
}

void UStatsAnimInstance::UWeaponAnimInstance()
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
