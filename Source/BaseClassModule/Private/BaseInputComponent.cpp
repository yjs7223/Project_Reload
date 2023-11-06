// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInputComponent.h"
#include "GameFramework/Character.h"
#include "BaseCharacterMovementComponent.h"
#include "CoverComponent.h"
#include "WeaponComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Engine/InputDelegateBinding.h"

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
	owner->SetupPlayerInputComponent(this);
	if (UInputDelegateBinding::SupportsInputDelegate(owner->GetClass()))
	{
		bBlockInput = bBlockInput;
		UInputDelegateBinding::BindInputDelegatesWithSubojects(owner, this);
	}
}

void UBaseInputComponent::UnBindInput()
{
	ClearActionBindings();
	KeyBindings.Reset();
	TouchBindings.Reset();
	AxisBindings.Reset();
	AxisKeyBindings.Reset();
	VectorAxisBindings.Reset();
	GestureBindings.Reset();
}

FInputData* UBaseInputComponent::getInput()
{
	return &m_inputData;
}

void UBaseInputComponent::ClearInput()
{
	m_inputData = FInputData();
}

void UBaseInputComponent::MoveForward(float Value)
{
	if(bIsbCrowdControl) return;
	m_inputData.movevec.X = Value;
}

void UBaseInputComponent::MoveRight(float Value)
{
	if (bIsbCrowdControl) return;
	m_inputData.movevec.Y = Value;
}

void UBaseInputComponent::Crouching()
{
	if (bIsbCrowdControl) return;
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
	if (bIsbCrowdControl) return;
	if (m_Covercomponent->IsCover()) return;
	if (m_inputData.IsReload) return;

	if (m_Movement->isRuning()) {
		m_Movement->SetMovementMode(MOVE_Walking);
	}
	else if(m_Movement->MovementMode != MOVE_Falling && m_Movement->MovementMode != MOVE_Flying) {
		m_Movement->SetMovementMode(MOVE_Custom, CMOVE_Runing);
		m_inputData.IsAiming = false;
	}
}

void UBaseInputComponent::StartFire()
{
	if (bIsbCrowdControl) return;
	if (m_Movement->isRuning()) {
		m_Movement->SetMovementMode(MOVE_Walking);
	}
	if (!m_inputData.IsReload)
	{
		m_inputData.IsFire = true;
		m_Weapon->StartFire();
	}
}

void UBaseInputComponent::StopFire()
{
	m_inputData.IsFire = false;
	m_Weapon->StopFire();
}

void UBaseInputComponent::StartAiming()
{
	if (bIsbCrowdControl) return;
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
	if (bIsbCrowdControl) return;
	if (!m_Weapon->CanReload()) return;
	if (m_Movement->isRuning()) {
		m_Movement->SetMovementMode(MOVE_Walking);
	}
	m_inputData.IsReload = true;
}

void UBaseInputComponent::StopReload()
{
	m_inputData.IsReload = false;
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

void UBaseInputComponent::SetCrowdControl(bool IsControl)
{
	bIsbCrowdControl = IsControl;
}

