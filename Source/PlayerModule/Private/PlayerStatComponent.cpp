// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatComponent.h"

UPlayerStatComponent::UPlayerStatComponent()
{

}

void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerStatComponent::BeginDestroy()
{
	OnVisibleHPUIDelegate.Unbind();
	OnChangedHealthDelegate.Unbind();



	Super::BeginDestroy();
}

void UPlayerStatComponent::SetHP(float p_HP)
{
	Super::SetHP(p_HP);

	OnChangedHealthDelegate.ExecuteIfBound(curHP / maxHP);
}

void UPlayerStatComponent::Attacked(float p_damage)
{
	Super::Attacked(p_damage);

	OnVisibleHPUIDelegate.ExecuteIfBound();
	OnChangedHealthDelegate.ExecuteIfBound(curHP / maxHP);
}
