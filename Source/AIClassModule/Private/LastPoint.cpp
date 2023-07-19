// Fill out your copyright notice in the Description page of Project Settings.


#include "LastPoint.h"

// Sets default values
ALastPoint::ALastPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALastPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALastPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

