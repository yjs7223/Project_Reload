// Fill out your copyright notice in the Description page of Project Settings.


#include "AIInputComponent.h"
#include "GameFramework/Character.h"
#include "CoverComponent.h"
#include "AIWeaponComponent.h"
#include "AICharacterMoveComponent.h"

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
	m_inputData.IsRuning = true;
}

void UAIInputComponent::AIStopRuning()
{
	m_inputData.IsRuning = false;
}

void UAIInputComponent::AICrouching()
{
	UCoverComponent* cover = owner->FindComponentByClass<UCoverComponent>();

	cover->m_IsCover = true;

	if (owner->CanCrouch()) {
		owner->Crouch();
	}
}

void UAIInputComponent::AIStopCrouching()
{
	UCoverComponent* cover = owner->FindComponentByClass<UCoverComponent>();

	cover->m_IsCover = false;
	owner->UnCrouch();

}

void UAIInputComponent::AIStartFire()
{
	UAIWeaponComponent* weaponcmp = owner->FindComponentByClass<UAIWeaponComponent>();
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	weaponcmp->ShotAIStart();
	weaponcmp->ShotAI();
	moveoncmp->e_move = EMove::Attack;
	m_inputData.IsFire = true;
}

void UAIInputComponent::AIStopFire()
{
	UAIWeaponComponent* weaponcmp = owner->FindComponentByClass<UAIWeaponComponent>();
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	weaponcmp->ShotAIStop();
	moveoncmp->e_move = EMove::Nomal;
	m_inputData.IsFire = false;

}

void UAIInputComponent::AIStartAiming()
{
	m_inputData.IsAiming = true;
}

void UAIInputComponent::AIStopAiming()
{
	m_inputData.IsAiming = false;
}

void UAIInputComponent::AIStartReload()
{
	m_inputData.IsReload = true;
}
