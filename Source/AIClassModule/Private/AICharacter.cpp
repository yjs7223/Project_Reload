// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "AICharacterMoveComponent.h"

AAICharacter::AAICharacter()
{
	AIMovement = CreateDefaultSubobject<UAICharacterMoveComponent>(TEXT("AIMovement"));
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
