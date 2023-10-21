// Fill out your copyright notice in the Description page of Project Settings.
#include "StatsAnimInstance.h"
#include "GameFramework/Character.h"
#include "WeaponComponent.h"
#include "StatComponent.h"
#include "Pakurable.h"


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
		FOnMontageEnded temp = FOnMontageEnded::CreateUObject(this, &UStatsAnimInstance::KnockBackEnd);
		owner->GetMesh()->GetAnimInstance()->Montage_Play(currmontage);
		if (m_PakurComp) {
			owner->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(temp, currmontage);
			if (m_PakurComp && m_PakurComp->GetClass()->ImplementsInterface(UPakurable::StaticClass())) {
				IPakurable::Execute_SetCanRolling(m_PakurComp, false);
			}
		}
		});

	TArray<UActorComponent*> pakurArr = owner->GetComponentsByInterface(UPakurable::StaticClass());
	if (pakurArr.Num() == 1) {
		m_PakurComp = pakurArr[0];
	}

	owner->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UStatsAnimInstance::KnockBackEnd);

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

void UStatsAnimInstance::KnockBackEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage->IsValidSlot("AdditiveHitReact")) {
		if (m_PakurComp && m_PakurComp->GetClass()->ImplementsInterface(UPakurable::StaticClass())) {
			IPakurable::Execute_SetCanRolling(m_PakurComp, true);
		}
	}
}
