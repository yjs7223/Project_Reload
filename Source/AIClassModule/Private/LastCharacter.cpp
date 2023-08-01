// Fill out your copyright notice in the Description page of Project Settings.


#include "LastCharacter.h"

// Sets default values
ALastCharacter::ALastCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALastCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALastCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALastCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

