// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleyPlayerCharacter.h"
#include "VehicleInputComponent.h"

AVehicleyPlayerCharacter::AVehicleyPlayerCharacter(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer.SetDefaultSubobjectClass<UVehicleInputComponent>(InputComponentName))
{
}
