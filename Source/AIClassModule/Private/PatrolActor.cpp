// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolActor.h"

// Sets default values
APatrolActor::APatrolActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APatrolActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APatrolActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

