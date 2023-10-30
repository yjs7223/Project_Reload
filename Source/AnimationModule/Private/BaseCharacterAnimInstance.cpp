// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "WeaponComponent.h"
#include "StatComponent.h"
#include "BaseInputComponent.h"
#include "CoverComponent.h"
#include "Pakurable.h"
#include "BaseCharacterMovementComponent.h"

UBaseCharacterAnimInstance::UBaseCharacterAnimInstance()
{
	m_HandGrip_R = TEXT("HandGripTransformRight");
	m_HandGrip_L = TEXT("HandGripTransformLeft");
}

void UBaseCharacterAnimInstance::NativeBeginPlay()
{
	m_Owner = Cast<ACharacter>(TryGetPawnOwner());
	m_Cover = m_Owner->FindComponentByClass<UCoverComponent>();
	m_Weapon = m_Owner->FindComponentByClass<UWeaponComponent>();
	m_Input = m_Owner->FindComponentByClass<UBaseInputComponent>();
	m_Stats = m_Owner->FindComponentByClass<UStatComponent>();
	m_Movement = m_Owner->FindComponentByClass<UBaseCharacterMovementComponent>();

	TArray<UActorComponent*> pakurArr = m_Owner->GetComponentsByInterface(UPakurable::StaticClass());
	if(pakurArr.Num() == 1) {
		m_PakurComp = pakurArr[0];
	}
}

void UBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!(m_Cover)) return;
	if (!(m_Weapon)) return;
	if (!(m_Input)) return;
	if (!(m_Stats)) return;
	if (!(m_PakurComp)) return;
	if (!m_PakurComp->GetClass()) return;


	m_IsFaceRight = m_Cover->IsFaceRight();
	m_IsReloding = m_Weapon->bReload;
	m_IsFire = m_Input->getInput()->IsFire;
	m_IsAiming = m_Input->getInput()->IsAiming;

	m_GunIKVec = m_Weapon->WeaponMesh->GetSocketLocation(m_IsFaceRight ? m_HandGrip_L : m_HandGrip_R);
	m_IsPakuru = IPakurable::Execute_IsRolling(m_PakurComp);


	m_IsHandIK = 
		(m_GunIKVec != FVector::ZeroVector) &&
		!m_IsReloding &&
		!m_Movement->isRuning() &&
		!m_Stats->bAttacked;


}
