// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleInputComponent.h"
#include "GameFramework/Character.h"

void UVehicleInputComponent::BindInput()
{
	UBaseInputComponent::BindInput();

	BindAction("Fire", IE_Pressed, this, &UVehicleInputComponent::StartFire);
	BindAction("Fire", IE_Released, this, &UVehicleInputComponent::StopFire);

	BindAxis("Turn Right / Left Mouse", owner, &ACharacter::AddControllerYawInput);
	BindAxis("Look Up / Down Mouse", owner, &ACharacter::AddControllerPitchInput);

	BindAction("Aim", IE_Pressed, this, &UPlayerInputComponent::StartAiming);
	BindAction("Aim", IE_Released, this, &UPlayerInputComponent::StopAiming);

	UBaseInputComponent::StartAiming();
}

void UVehicleInputComponent::StartAiming()
{
	m_inputData.IsAiming = true;
}

void UVehicleInputComponent::StopAiming()
{
	m_inputData.IsAiming = false;
}
