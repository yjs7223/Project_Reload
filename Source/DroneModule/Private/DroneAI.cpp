// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneAI.h"

// Sets default values
ADroneAI::ADroneAI()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADroneAI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADroneAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADroneAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

