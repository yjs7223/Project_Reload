// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatComponent.h"
#include "BaseCharacter.h"
#include "MatineeCameraShake.h"

UPlayerStatComponent::UPlayerStatComponent()
{
	TargetEnemy = nullptr;
}

void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerStatComponent::BeginDestroy()
{
	OnVisibleHPUIDelegate.Clear();
	OnChangedHealthDelegate.Clear();



	Super::BeginDestroy();
}

void UPlayerStatComponent::SetHP(float p_HP)
{
	Super::SetHP(p_HP);

	OnChangedHealthDelegate.Broadcast(curHP / maxHP);
	//OnChangedHealthDelegate.ExecuteIfBound(curHP / maxHP);
}

void UPlayerStatComponent::RecoverHP(float p_HP)
{
	Super::RecoverHP(p_HP);

	OnChangedHealthDelegate.Broadcast(curHP / maxHP);
}

void UPlayerStatComponent::Attacked(ABaseCharacter* attacker, float p_damage, EHitType hittype, FVector attackPoint)
{
	Super::Attacked(attacker, p_damage, hittype, attackPoint);

	TargetEnemy = attacker;

	if (AttackedCameraShake)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(AttackedCameraShake, 1.0f);

	}

	OnVisibleHPUIDelegate.Broadcast();
	OnChangedHealthDelegate.Broadcast(curHP / maxHP);
	OnVisibleAttackedUIDelegate.ExecuteIfBound();
}

//void UPlayerStatComponent::Attacked(float p_damage)
//{
//	Super::Attacked(p_damage);
//
//	if (AttackedCameraShake)
//	{
//		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(AttackedCameraShake, 1.0f);
//
//	}
//
//	OnVisibleHPUIDelegate.Broadcast();
//	OnChangedHealthDelegate.Broadcast(curHP / maxHP);
//	OnVisibleAttackedUIDelegate.ExecuteIfBound();
//	
//}
//
//void UPlayerStatComponent::Attacked(float p_damage, ACharacter* character)
//{
//	Super::Attacked(p_damage, character);
//
//	TargetEnemy = character;
//
//	if (AttackedCameraShake)
//	{
//		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(AttackedCameraShake, 1.0f);
//	}
//
//	OnVisibleHPUIDelegate.Broadcast();
//	OnChangedHealthDelegate.Broadcast(curHP / maxHP);
//	OnVisibleAttackedUIDelegate.ExecuteIfBound();
//}

