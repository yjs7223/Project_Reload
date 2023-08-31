// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInputComponent.h"
#include "GameFramework/Character.h"
#include "PlayerWeaponComponent.h"
#include "PlayerStatComponent.h"
#include "CoverComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include "PlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "PlayerHUDWidget.h"
#include "Player_HP_Widget.h"
#include "BaseCharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

void UPlayerInputComponent::BeginPlay()
{
	Super::BeginPlay();
	TObjectPtr<class UInputComponent> InputComponent = owner->InputComponent;

	InputComponent->BindAxis("Move Forward / Backward", this, &UPlayerInputComponent::MoveForward);
	InputComponent->BindAxis("Move Right / Left", this, &UPlayerInputComponent::MoveRight);
	InputComponent->BindAction("Move", IE_Pressed, this, &UPlayerInputComponent::InputMove);

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
	InputComponent->BindAction("Aim", IE_Released, covercomp, &UCoverComponent::StopPeeking);

	InputComponent->BindAction("TestInput", IE_Pressed, this, &UPlayerInputComponent::TestHud);
	InputComponent->BindAction("HP_regen", IE_Pressed, this, &UPlayerInputComponent::HPregen);

	APlayerCharacter* pc = Cast<APlayerCharacter>(owner);
	InputComponent->BindAction("UI_Visible", IE_Pressed, pc, &APlayerCharacter::WidgetShow);
}

void UPlayerInputComponent::MoveForward(float Value)
{
	m_inputData.movevec.X = Value;
}

void UPlayerInputComponent::MoveRight(float Value)
{
	m_inputData.movevec.Y = Value;
}

void UPlayerInputComponent::InputMove()
{
	UCoverComponent* covercomp = owner->FindComponentByClass<UCoverComponent>();
	if (!covercomp->IsCover()) {
		covercomp->StopCover();
	}
}

void UPlayerInputComponent::Runing()
{
	UBaseCharacterMovementComponent* movement = owner->FindComponentByClass<UBaseCharacterMovementComponent>();
	UCoverComponent* covercomp = owner->FindComponentByClass<UCoverComponent>();
	if (covercomp->IsCover()) return;

	if (movement->isRuning()) {
		owner->FindComponentByClass<UBaseCharacterMovementComponent>()->SetMovementMode(MOVE_Walking);
	}
	else {
		owner->FindComponentByClass<UBaseCharacterMovementComponent>()->SetMovementMode(MOVE_Custom, CMOVE_Runing);
	}

	if (movement->isRuning()) {
		m_inputData.IsAiming = false;
	}
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
	UBaseCharacterMovementComponent* movement = owner->FindComponentByClass<UBaseCharacterMovementComponent>();
	m_inputData.IsAiming = true;
	if (movement->isRuning()) {
		movement->SetMovementMode(MOVE_Walking);
	}
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

void UPlayerInputComponent::TestHud()
{
	owner->FindComponentByClass<UStatComponent>()->Attacked(20.0f);
}

void UPlayerInputComponent::HPregen()
{
	owner->FindComponentByClass<UStatComponent>()->RecoverHP(100000.0f);
}
