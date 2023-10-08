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
	movement = owner->FindComponentByClass<UBaseCharacterMovementComponent>();
	covercomp = owner->FindComponentByClass<UCoverComponent>();
	AI_Crouch = false;
	AI_Cover = false;
	AI_Running = false;
	AI_Fire = false;
	AI_Aiming = false;
	AI_Reload = false;
}

void UAIInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	AI_Aiming = m_inputData.IsAiming;
	AI_Fire = m_inputData.IsFire;
	AI_Reload = m_inputData.IsReload;
	AI_Running = movement->isRuning();
	AI_Crouch = owner->bIsCrouched;
	AI_Cover = covercomp->IsCover();
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
	movement->SetMovementMode(MOVE_Custom, CMOVE_Runing);
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	if (moveoncmp->e_move != EMove::Hit && moveoncmp->e_move != EMove::Stun)
	{
		moveoncmp->e_move = EMove::Normal;
	}
}

void UAIInputComponent::AIStopRuning()
{
	movement->SetMovementMode(MOVE_Walking);
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	if (moveoncmp->e_move != EMove::Hit && moveoncmp->e_move != EMove::Stun)
	{
		moveoncmp->e_move = EMove::Normal;
	}
}

void UAIInputComponent::AICrouching()
{
	if (owner->CanCrouch()) {
		owner->Crouch();
	}
}

void UAIInputComponent::AIStopCrouching()
{
	owner->UnCrouch();
}

void UAIInputComponent::AIStartFire()
{
	UAIWeaponComponent* weaponcmp = owner->FindComponentByClass<UAIWeaponComponent>();
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	weaponcmp->StartFire();
	weaponcmp->Fire();
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
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	if (moveoncmp->e_move != EMove::Hit && moveoncmp->e_move != EMove::Stun)
	{
		moveoncmp->e_move = EMove::Normal;
	}
	m_inputData.IsFire = false;

}

void UAIInputComponent::AIStartAiming()
{
	UAIWeaponComponent* weapon = owner->FindComponentByClass<UAIWeaponComponent>();

	
	if (movement->isRuning()) {
		movement->SetMovementMode(MOVE_Walking);
	}
	
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	if (moveoncmp->e_move != EMove::Hit && moveoncmp->e_move != EMove::Stun)
	{
		moveoncmp->e_move = EMove::Attack;
	}

	covercomp->StartPeeking();
	weapon->AimFalshOn();
	m_inputData.IsAiming = true;
}

void UAIInputComponent::AIStopAiming()
{
	UAIWeaponComponent* weapon = owner->FindComponentByClass<UAIWeaponComponent>();

	m_inputData.IsAiming = false;
	
	covercomp->StopPeeking();
	weapon->AimFalshOff();
}

void UAIInputComponent::AIStartReload()
{
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	if (moveoncmp->e_move != EMove::Hit && moveoncmp->e_move != EMove::Stun)
	{
		moveoncmp->e_move = EMove::Attack;
	}
	m_inputData.IsReload = true;

}

void UAIInputComponent::AIStopReload()
{
	m_inputData.IsReload = false;
}

void UAIInputComponent::AIStartCover()
{
	covercomp->StartAICover();
}

void UAIInputComponent::AIStopCover()
{
	covercomp->StopCover();
}
