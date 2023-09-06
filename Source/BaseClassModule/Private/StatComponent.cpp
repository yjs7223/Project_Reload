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
	HitReactionScale = 0.2f;
	// ...
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner<ABaseCharacter>();
	// ...
	
}

void UStatComponent::BeginDestroy()
{
	Super::BeginDestroy();
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
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(curHP));
}

//void UStatComponent::Attacked_BP(float p_damage, FVector attackPoint, EHitType hittype)
//{
//	curHP -= p_damage;
//	bAttacked = true;
//	if (curHP < 0.0f)
//	{
//		curHP = 0.0f;
//		bDie = true;
//		diePlay.Broadcast();
//	}
//	switch (hittype)
//	{
//	case EHitType::None:
//		break;
//	case EHitType::Normal:
//		break;
//	case EHitType::Knockback:
//		Knockback.Broadcast(attackPoint, bDie);
//		break;
//	case EHitType::MAX:
//		break;
//	default:
//		break;
//	}
//}
//
//void UStatComponent::Attacked(float p_damage)
//{
//	curHP -= p_damage;
//	bAttacked = true;
//	if (curHP < 0.0f)
//	{
//		curHP = 0.0f;
//		bDie = true;
//		diePlay.Broadcast();
//	}
//	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(curHP));
//}
//
//void UStatComponent::Attacked(float p_damage, FHitResult result)
//{
//
//	curHP -= p_damage;
//	bAttacked = true;
//	if (curHP < 0.0f)
//	{
//		curHP = 0.0f;
//		bDie = true;
//		diePlay.Broadcast();
//	}
//
//}
//
//void UStatComponent::Attacked(float p_damage, ACharacter* character)
//{
//	curHP -= p_damage;
//	bAttacked = true;
//	if (curHP < 0.0f)
//	{
//		curHP = 0.0f;
//		bDie = true;
//		diePlay.Broadcast();
//	}
//}
//
//void UStatComponent::Attacked(FHitResult result)
//{
//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("indirection hit"));
//}

void UStatComponent::Attacked_BP(float p_damage, ABaseCharacter* attacker, EHitType hittype, FVector attackPoint)
{
	Attacked(p_damage, attacker, hittype, attackPoint);
}

void UStatComponent::Attacked(float p_damage, ABaseCharacter* attacker, EHitType hittype, FVector attackPoint)
{
	curHP -= p_damage;
	bAttacked = true;
	if (curHP < 0.0f)
	{
		curHP = 0.0f;
		bDie = true;
		diePlay.Broadcast();
	}

	switch (hittype)
	{
	case EHitType::None:
		break;
	case EHitType::Normal:
		break;
	case EHitType::Knockback:
		Knockback.Broadcast(attackPoint, bDie);
		break;
	case EHitType::MAX:
		break;
	default:
		break;
	}
}

void UStatComponent::IndirectAttacked(float p_Value)
{

}



