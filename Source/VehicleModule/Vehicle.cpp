// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"
#include "ChaosVehicleMovementComponent.h"
#include "Components/SplineComponent.h"

AVehicle::AVehicle(const FObjectInitializer& ObjectInitializer)
{
	
}

void AVehicle::BeginPlay()
{
	Super::BeginPlay();
	vehicleMovementComp = FindComponentByClass<UChaosVehicleMovementComponent>();
	vehicleMovementComp->SetThrottleInput(1.0f);
}

void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (deadCar)
	{

	}
}

void AVehicle::FindPath(USplineComponent spline)
{
}
