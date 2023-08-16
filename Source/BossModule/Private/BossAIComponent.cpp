// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIComponent.h"

// Sets default values for this component's properties
UBossAIComponent::UBossAIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBossAIComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UBossAIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBossAIComponent::Attack(float p_Damage)
{
	UE_LOG(LogTemp, Warning, TEXT("Attack function called from Blueprint!"));
}
