// Fill out your copyright notice in the Description page of Project Settings.


#include "AIInputComponent.h"
#include "GameFramework/Character.h"
#include "CoverComponent.h"
#include "AIWeaponComponent.h"
#include "AICharacterMoveComponent.h"
#include "BaseCharacterMovementComponent.h"
#include "AIWeaponComponent.h"

void UAIInputComponent::BeginPlay()
{
	Super::BeginPlay();

	UCoverComponent* covercomp = owner->FindComponentByClass<UCoverComponent>();
}

void UAIInputComponent::AIMoveForward(float Value)
{
	m_inputData.movevec.X = Value;
}

void UAIInputComponent::AIMoveRight(float Value)
{
	m_inputData.movevec.Y = Value;
}

void UAIInputComponent::AIRuning()
{
	UBaseCharacterMovementComponent* movement = owner->FindComponentByClass<UBaseCharacterMovementComponent>();
	movement->SetMovementMode(MOVE_Custom, CMOVE_Runing);
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	if (moveoncmp->e_move != EMove::Hit)
	{
		moveoncmp->e_move = EMove::Normal;
	}
}

void UAIInputComponent::AIStopRuning()
{
	UBaseCharacterMovementComponent* movement = owner->FindComponentByClass<UBaseCharacterMovementComponent>();
	movement->SetMovementMode(MOVE_Walking);
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	if (moveoncmp->e_move != EMove::Hit)
	{
		moveoncmp->e_move = EMove::Normal;
	}
}

void UAIInputComponent::AICrouching()
{
	UCoverComponent* cover = owner->FindComponentByClass<UCoverComponent>();


	if (owner->CanCrouch()) {
		owner->Crouch();
	}
}

void UAIInputComponent::AIStopCrouching()
{
	UCoverComponent* cover = owner->FindComponentByClass<UCoverComponent>();

	owner->UnCrouch();

}

void UAIInputComponent::AIStartFire()
{
	UAIWeaponComponent* weaponcmp = owner->FindComponentByClass<UAIWeaponComponent>();
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	weaponcmp->ShotAIStart();
	weaponcmp->Fire();

	if (moveoncmp->e_move != EMove::Hit)
	{
		moveoncmp->e_move = EMove::Attack;
	}
	
	m_inputData.IsFire = true;
}

void UAIInputComponent::AIStopFire()
{
	UAIWeaponComponent* weaponcmp = owner->FindComponentByClass<UAIWeaponComponent>();
	weaponcmp->ShotAIStop();
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	if (moveoncmp->e_move != EMove::Hit)
	{
		moveoncmp->e_move = EMove::Normal;
	}
	m_inputData.IsFire = false;

}

void UAIInputComponent::AIStartAiming()
{
	UCoverComponent* cover = owner->FindComponentByClass<UCoverComponent>();
	UAIWeaponComponent* weapon = owner->FindComponentByClass<UAIWeaponComponent>();


	cover->StartPeeking();
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	if (moveoncmp->e_move != EMove::Hit)
	{
		moveoncmp->e_move = EMove::Attack;
	}

	m_inputData.IsAiming = true;
	weapon->AimFalshOn();
	
}

void UAIInputComponent::AIStopAiming()
{
	UCoverComponent* cover = owner->FindComponentByClass<UCoverComponent>();
	UAIWeaponComponent* weapon = owner->FindComponentByClass<UAIWeaponComponent>();

	m_inputData.IsAiming = false;
	
	cover->StopPeeking();
	weapon->AimFalshOff();
}

void UAIInputComponent::AIStartReload()
{
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	moveoncmp->e_move = EMove::Attack;
	m_inputData.IsReload = true;

}

void UAIInputComponent::AIStopReload()
{
	m_inputData.IsReload = false;
}

void UAIInputComponent::AIStartCover()
{
	UCoverComponent* cover = owner->FindComponentByClass<UCoverComponent>();

	cover->StartAICover();
}

void UAIInputComponent::AIStopCover()
{
	UCoverComponent* cover = owner->FindComponentByClass<UCoverComponent>();

	cover->StopCover();
}
