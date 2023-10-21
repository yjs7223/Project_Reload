// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "Vehicle.generated.h"

/**
 * 
 */
UCLASS()
class VEHICLEMODULE_API AVehicle : public AWheeledVehiclePawn
{
	GENERATED_BODY()
public:
	AVehicle(const FObjectInitializer& ObjectInitializer);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

};
