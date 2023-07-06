// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"
#include "BaseCharacter.h"
#include "Components/InputComponent.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

ABaseCharacter* UStatComponent::GetCharacter()
{
	return owner;
}

void UStatComponent::SetHP(float p_HP)
{
	maxHP = p_HP;
	curHP = p_HP;
}

void UStatComponent::RecoverHP(float p_HP)
{
	curHP += p_HP;
	if (curHP > maxHP)
	{
		curHP = maxHP;
	}
}

void UStatComponent::Attacked(float p_damage)
{
	curHP -= p_damage;
	isAttacked = true;
	if (curHP < 0.0f)
	{
		curHP = 0.0f;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(curHP));
}

void UStatComponent::Attacked(float p_damage, FHitResult result)
{
}

