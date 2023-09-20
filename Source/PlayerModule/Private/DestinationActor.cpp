// Fill out your copyright notice in the Description page of Project Settings.


#include "DestinationActor.h"

// Sets default values
ADestinationActor::ADestinationActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADestinationActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestinationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

