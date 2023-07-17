// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInputComponent.h"
#include "GameFramework/Character.h"
#include "PlayerWeaponComponent.h"
#include "CoverComponent.h"
#include <Kismet/KismetSystemLibrary.h>

void UPlayerInputComponent::BeginPlay()
{
	Super::BeginPlay();
	TObjectPtr<class UInputComponent> InputComponent = owner->InputComponent;

	InputComponent->BindAxis("Move Forward / Backward", this, &UPlayerInputComponent::MoveForward);
	InputComponent->BindAxis("Move Right / Left", this, &UPlayerInputComponent::MoveRight);

	InputComponent->BindAxis("Turn Right / Left Mouse", owner, &ACharacter::AddControllerYawInput);
	InputComponent->BindAxis("Look Up / Down Mouse", owner, &ACharacter::AddControllerPitchInput);

	InputComponent->BindAction("Crouch", IE_Pressed, this, &UPlayerInputComponent::Crouching);
	InputComponent->BindAction("Runing", IE_Pressed, this, &UPlayerInputComponent::Runing);

	InputComponent->BindAction("Fire", IE_Pressed, this, &UPlayerInputComponent::StartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &UPlayerInputComponent::StopFire);

	InputComponent->BindAction("Aim", IE_Pressed, this, &UPlayerInputComponent::StartAiming);
	InputComponent->BindAction("Aim", IE_Released, this, &UPlayerInputComponent::StopAiming);

	InputComponent->BindAction("Reload", IE_Pressed, this, &UPlayerInputComponent::StartReload);


	UCoverComponent* covercomp = owner->FindComponentByClass<UCoverComponent>();
	InputComponent->BindAction("Cover", IE_Pressed, covercomp, &UCoverComponent::PlayCover);
	InputComponent->BindAction("Aim", IE_Pressed, covercomp, &UCoverComponent::StartPeeking);
	InputComponent->BindAction("Aim", IE_Pressed, covercomp, &UCoverComponent::StopPeeking);
}

void UPlayerInputComponent::MoveForward(float Value)
{
	m_inputData.movevec.X = Value;
}

void UPlayerInputComponent::MoveRight(float Value)
{
	m_inputData.movevec.Y = Value;
}

void UPlayerInputComponent::Runing()
{
	m_inputData.IsRuning ? m_inputData.IsRuning = false : m_inputData.IsRuning = true;
}

void UPlayerInputComponent::Crouching()
{
	if (owner->CanCrouch()) {
		owner->Crouch();
	}
	else {
		if (!m_CanUnCrouch) return;
		owner->UnCrouch();
	}
}

void UPlayerInputComponent::StartFire()
{
	if (!m_inputData.IsReload)
	{
		owner->FindComponentByClass<UPlayerWeaponComponent>()->StartFire();
		if (owner->FindComponentByClass<UPlayerWeaponComponent>()->isFire)
		{
			m_inputData.IsFire = true;
		}
	}
}

void UPlayerInputComponent::StopFire()
{
	m_inputData.IsFire = false;
	owner->FindComponentByClass<UPlayerWeaponComponent>()->StopFire();
}

void UPlayerInputComponent::StartAiming()
{
	m_inputData.IsAiming = true;
	owner->FindComponentByClass<UPlayerWeaponComponent>()->StartAiming();
}

void UPlayerInputComponent::StopAiming()
{
	m_inputData.IsAiming = false;
	owner->FindComponentByClass<UPlayerWeaponComponent>()->StopAiming();
}

void UPlayerInputComponent::StartReload()
{
	owner->FindComponentByClass<UPlayerWeaponComponent>()->StartReload();
	if (owner->FindComponentByClass<UPlayerWeaponComponent>()->isReload)
	{
		m_inputData.IsReload = true;
	}
}
