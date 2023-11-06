// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"
#include "BaseCharacter.h"
#include "Components/InputComponent.h"
#include "BaseInputComponent.h"

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
	m_Input = owner ? owner->FindComponentByClass<UBaseInputComponent>() : nullptr;
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
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(curHP));
	
}

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

	FTimerHandle stuntimer;
	switch (hittype)
	{
	case EHitType::None:
		break;
	case EHitType::Normal:
		break;
	case EHitType::Knockback:
		Knockback.Broadcast(attackPoint, bDie);
		bIsKnockback = true;
		break;
	case EHitType::Stun:
		bIsStun = true;
		
		owner->GetWorldTimerManager().SetTimer(stuntimer, 
			[this]() 
			{
				bIsStun = false; 
			}
		, 3.0f, false);
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

void UStatComponent::SetKnockback(bool p_Knockback)
{
	bIsKnockback = p_Knockback;
	if (m_Input) m_Input->SetCrowdControl(p_Knockback);
}



