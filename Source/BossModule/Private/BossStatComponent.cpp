// Fill out your copyright notice in the Description page of Project Settings.


#include "BossStatComponent.h"

UBossStatComponent::UBossStatComponent()
{
}

void UBossStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBossStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
}

/*void UBossStatComponent::Attacked(float p_damage)
{
	curHP -= p_damage;
	bAttacked = true;
	if (curHP < 0.0f)
	{
		curHP = 0.0f;
		bDie = true;
	}
}

void UBossStatComponent::Attacked(float p_damage, FHitResult result)
{
	curHP -= p_damage;
	bAttacked = true;
	if (curHP < 0.0f)
	{
		curHP = 0.0f;
		bDie = true;
	}
}

void UBossStatComponent::Attacked(FHitResult result)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("indirection hit"));
}*/
