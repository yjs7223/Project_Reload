// Fill out your copyright notice in the Description page of Project Settings.


#include "BossWeaponComponent.h"
#include "StatComponent.h"
#include "BaseCharacter.h"

// Sets default values for this component's properties
UBossWeaponComponent::UBossWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBossWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBossWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBossWeaponComponent::TakeDamage(AActor* p_TargetActor, float p_Damage)
{
	auto target = p_TargetActor->FindComponentByClass<UStatComponent>();
	if (target)
	{
		target->Attacked(p_Damage, GetOwner<ACharacter>());
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, GetOwner<ABaseCharacter>()->GetName());
		//target->Attacked(p_Damage);
	}
}

