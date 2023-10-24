// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInputComponent.h"
#include "GameFramework/Character.h"
#include "BaseCharacterMovementComponent.h"
#include "CoverComponent.h"
#include "WeaponComponent.h"
#include "Navigation/PathFollowingComponent.h"

// Called when the game starts
void UBaseInputComponent::BeginPlay()
{
	Super::BeginPlay();

	owner = GetOwner<ACharacter>();

	m_Weapon = owner->FindComponentByClass<UWeaponComponent>();

	m_Covercomponent = owner->FindComponentByClass<UCoverComponent>();
	m_Movement = owner->FindComponentByClass<UBaseCharacterMovementComponent>();

	m_PathFollowingComp = owner->GetController()->FindComponentByClass<UPathFollowingComponent>();

	SetInputEnable(true);
}

void UBaseInputComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SetInputEnable(false);
}

void UBaseInputComponent::BindInput()
{
}

void UBaseInputComponent::UnBindInput()
{
}

FInputData* UBaseInputComponent::getInput()
{
	return &m_inputData;
}

void UBaseInputComponent::MoveForward(float Value)
{
	m_inputData.movevec.X = Value;
}

void UBaseInputComponent::MoveRight(float Value)
{
	m_inputData.movevec.Y = Value;
}

void UBaseInputComponent::Crouching()
{
	if (owner->CanCrouch()) {
		owner->Crouch();
	}
	else {
		if (!m_CanUnCrouch) return;
		owner->UnCrouch();
	}
}

void UBaseInputComponent::Runing()
{
	if (m_Covercomponent->IsCover()) return;
	if (m_inputData.IsReload) return;

	if (m_Movement->isRuning()) {
		m_Movement->SetMovementMode(MOVE_Walking);
	}
	else if(m_Movement->MovementMode != MOVE_Falling) {
		m_Movement->SetMovementMode(MOVE_Custom, CMOVE_Runing);
		m_inputData.IsAiming = false;
	}
}

void UBaseInputComponent::StartFire()
{
	if (m_Movement->isRuning()) {
		m_Movement->SetMovementMode(MOVE_Walking);
	}
	if (!m_inputData.IsReload)
	{
		m_Weapon->StartFire();
		if (m_Weapon->bFire)
		{
			m_inputData.IsFire = true;
		}
	}
}

void UBaseInputComponent::StopFire()
{
	m_inputData.IsFire = false;
	m_Weapon->StopFire();
}

void UBaseInputComponent::StartAiming()
{
	m_inputData.IsAiming = true;
	if (m_Movement->isRuning()) {
		m_Movement->SetMovementMode(MOVE_Walking);
	}
	m_Weapon->StartAiming();
}

void UBaseInputComponent::StopAiming()
{
	m_inputData.IsAiming = false;
	m_Weapon->StopAiming();
}

void UBaseInputComponent::StartReload()
{
	if (!m_Weapon->CanReload()) return;
	m_inputData.IsReload = true;
}

void UBaseInputComponent::SetInputEnable(bool IsEnable)
{
	if (IsEnable && !m_IsInputEnabled) {
		BindInput();
	}
	else if (!IsEnable) {
		UnBindInput();
	}

	m_IsInputEnabled = IsEnable;
}

