// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStatComponent.h"
#include "AICharacter.h"

UAIStatComponent::UAIStatComponent()
{
	
}

void UAIStatComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner<AAICharacter>();
}

void UAIStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
