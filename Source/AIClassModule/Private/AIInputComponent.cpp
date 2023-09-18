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
	AI_Crouch = false;
	AI_Cover = false;
	AI_Running = false;
	AI_Fire = false;
	AI_Aiming = false;
	AI_Reload = false;
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
	AI_Running = true;
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	if (moveoncmp->e_move != EMove::Hit && moveoncmp->e_move != EMove::Stun)
	{
		moveoncmp->e_move = EMove::Normal;
	}
}

void UAIInputComponent::AIStopRuning()
{
	UBaseCharacterMovementComponent* movement = owner->FindComponentByClass<UBaseCharacterMovementComponent>();
	movement->SetMovementMode(MOVE_Walking);
	AI_Running = false;
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	if (moveoncmp->e_move != EMove::Hit && moveoncmp->e_move != EMove::Stun)
	{
		moveoncmp->e_move = EMove::Normal;
	}
}

void UAIInputComponent::AICrouching()
{
	UCoverComponent* cover = owner->FindComponentByClass<UCoverComponent>();

	
	if (owner->CanCrouch()) {
		owner->Crouch();
		AI_Crouch = true;
		
	}
}

void UAIInputComponent::AIStopCrouching()
{
	UCoverComponent* cover = owner->FindComponentByClass<UCoverComponent>();
	owner->UnCrouch();
	AI_Crouch = false;
	

}

void UAIInputComponent::AIStartFire()
{
	UAIWeaponComponent* weaponcmp = owner->FindComponentByClass<UAIWeaponComponent>();
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	weaponcmp->StartFire();
	weaponcmp->Fire();
	AI_Fire = true;
	if (moveoncmp->e_move != EMove::Hit && moveoncmp->e_move != EMove::Stun)
	{
		moveoncmp->e_move = EMove::Attack;
	}
	
	m_inputData.IsFire = true;
}

void UAIInputComponent::AIStopFire()
{
	UAIWeaponComponent* weaponcmp = owner->FindComponentByClass<UAIWeaponComponent>();
	weaponcmp->StopFire();
	AI_Fire = false;
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	if (moveoncmp->e_move != EMove::Hit && moveoncmp->e_move != EMove::Stun)
	{
		moveoncmp->e_move = EMove::Normal;
	}
	m_inputData.IsFire = false;

}

void UAIInputComponent::AIStartAiming()
{
	UCoverComponent* cover = owner->FindComponentByClass<UCoverComponent>();
	UAIWeaponComponent* weapon = owner->FindComponentByClass<UAIWeaponComponent>();

	UBaseCharacterMovementComponent* movement = owner->FindComponentByClass<UBaseCharacterMovementComponent>();
	
	if (movement->isRuning()) {
		movement->SetMovementMode(MOVE_Walking);
	}
	
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	if (moveoncmp->e_move != EMove::Hit && moveoncmp->e_move != EMove::Stun)
	{
		moveoncmp->e_move = EMove::Attack;
	}

	cover->StartPeeking();
	weapon->AimFalshOn();
	AI_Aiming = true;
	m_inputData.IsAiming = true;
}

void UAIInputComponent::AIStopAiming()
{
	UCoverComponent* cover = owner->FindComponentByClass<UCoverComponent>();
	UAIWeaponComponent* weapon = owner->FindComponentByClass<UAIWeaponComponent>();

	m_inputData.IsAiming = false;
	
	cover->StopPeeking();
	weapon->AimFalshOff();
	AI_Aiming = false;
}

void UAIInputComponent::AIStartReload()
{
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	if (moveoncmp->e_move != EMove::Hit && moveoncmp->e_move != EMove::Stun)
	{
		moveoncmp->e_move = EMove::Attack;
	}
	AI_Reload = true;
	m_inputData.IsReload = true;

}

void UAIInputComponent::AIStopReload()
{
	AI_Reload = false;
	m_inputData.IsReload = false;
}

void UAIInputComponent::AIStartCover()
{
	UCoverComponent* cover = owner->FindComponentByClass<UCoverComponent>();

	cover->StartAICover();
	AI_Cover = true;
}

void UAIInputComponent::AIStopCover()
{
	UCoverComponent* cover = owner->FindComponentByClass<UCoverComponent>();

	cover->StopCover();
	AI_Cover = false;
}
