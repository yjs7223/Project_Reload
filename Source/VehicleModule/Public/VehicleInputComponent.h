// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerInputComponent.h"
#include "VehicleInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class VEHICLEMODULE_API UVehicleInputComponent : public UPlayerInputComponent
{
	GENERATED_BODY()

public:
	virtual void BindInput() override;

	virtual void StartAiming() override;
	virtual void StopAiming() override;

};
