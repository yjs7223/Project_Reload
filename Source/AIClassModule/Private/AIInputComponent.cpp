// Fill out your copyright notice in the Description page of Project Settings.


#include "AIInputComponent.h"
#include "GameFramework/Character.h"
#include "CoverComponent.h"

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
	m_inputData.IsRuning ? m_inputData.IsRuning = false : m_inputData.IsRuning = true;
}

void UAIInputComponent::AICrouching()
{
	if (owner->CanCrouch()) {
		owner->Crouch();
	}
	else {
		if (!m_CanUnCrouch) return;
		owner->UnCrouch();
	}
}

void UAIInputComponent::AIStartFire()
{
	m_inputData.IsFire = true;
}

void UAIInputComponent::AIStopFire()
{
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
