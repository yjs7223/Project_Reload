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
#include "Kismet/GameplayStatics.h"


void UPlayerInputComponent::BeginPlay()
{
	Super::BeginPlay();
	m_PlayerWeapon = owner->FindComponentByClass<UPlayerWeaponComponent>();
	m_Covercomponent = owner->FindComponentByClass<UCoverComponent>();
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

	InputComponent->BindAction("ChangeMainWeapon", IE_Pressed, this, &UPlayerInputComponent::ChangeMainWeapon);
	InputComponent->BindAction("ChangeSubWeapon", IE_Pressed, this, &UPlayerInputComponent::ChangeSubWeapon);

	InputComponent->BindAction("Cover", IE_Pressed, m_Covercomponent.Get(), &UCoverComponent::PlayCover);
	InputComponent->BindAction("Cover", IE_Released, this, &UPlayerInputComponent::StopCover);
	InputComponent->BindAction("Aim", IE_Pressed, m_Covercomponent.Get(), &UCoverComponent::StartPeeking);
	InputComponent->BindAction("Aim", IE_Released, m_Covercomponent.Get(), &UCoverComponent::StopPeeking);

	InputComponent->BindAction("HP_reduce", IE_Pressed, this, &UPlayerInputComponent::HPreduce);
	InputComponent->BindAction("HP_regen", IE_Pressed, this, &UPlayerInputComponent::HPregen);

	//APlayerCharacter* pc = Cast<APlayerCharacter>(owner);
	InputComponent->BindAction("UI_Visible", IE_Pressed, this, &UPlayerInputComponent::VisibleHud);
	InputComponent->BindAction("GamePause", IE_Pressed, this, &UPlayerInputComponent::GamePause);

	UPlayerStatComponent* statcomp = GetOwner()->FindComponentByClass<UPlayerStatComponent>();
	InputComponent->BindAction("Interactive", IE_Pressed, statcomp, &UPlayerStatComponent::Interacting);

	m_PathFollowingComp = owner->GetController()->FindComponentByClass<UPathFollowingComponent>();
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

	if (!m_Covercomponent->IsCover() && m_PathFollowingComp->GetStatus() == EPathFollowingStatus::Moving) {
		m_Covercomponent->StopCover();
	}
}

void UPlayerInputComponent::Runing()
{
	UBaseCharacterMovementComponent* movement = Cast<UBaseCharacterMovementComponent>(owner->GetCharacterMovement());

	if (m_Covercomponent->IsCover()) return;

	if (movement->isRuning()) {
		movement->SetMovementMode(MOVE_Walking);
	}
	else {
		movement->SetMovementMode(MOVE_Custom, CMOVE_Runing);
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
	OnCombatWidgetVisible.Broadcast(false);
	if (!m_inputData.IsReload)
	{
		m_PlayerWeapon->StartFire();
		if (m_PlayerWeapon->bFire)
		{
			m_inputData.IsFire = true;
		}
	}
}

void UPlayerInputComponent::StopFire()
{
	OnCombatWidgetVisible.Broadcast(true);
	m_inputData.IsFire = false;
	m_PlayerWeapon->StopFire();
}

void UPlayerInputComponent::StartAiming()
{
	OnCombatWidgetVisible.Broadcast(false);
	UBaseCharacterMovementComponent* movement = Cast<UBaseCharacterMovementComponent>(owner->GetCharacterMovement());
	m_inputData.IsAiming = true;
	if (movement->isRuning()) {
		movement->SetMovementMode(MOVE_Walking);
	}

	if (!m_Covercomponent->IsCover()) {
		m_Covercomponent->StopCover();
	}
	m_PlayerWeapon->StartAiming();
}

void UPlayerInputComponent::StopAiming()
{
	OnCombatWidgetVisible.Broadcast(true);
	m_inputData.IsAiming = false;
	m_PlayerWeapon->StopAiming();
}

void UPlayerInputComponent::ChangeMainWeapon()
{
	if (m_PlayerWeapon->weapontype == EWeaponType::TE_Rifle) return;
	m_PlayerWeapon->weapontype = EWeaponType::TE_Rifle;
	OnChangedWeapon.Broadcast();
}
	
void UPlayerInputComponent::ChangeSubWeapon()
{
	if (m_PlayerWeapon->weapontype == EWeaponType::TE_Pistol) return;
	m_PlayerWeapon->weapontype = EWeaponType::TE_Pistol;
	OnChangedWeapon.Broadcast();
}

void UPlayerInputComponent::StopCover()
{
	UBaseCharacterMovementComponent* movement = Cast<UBaseCharacterMovementComponent>(owner->GetCharacterMovement());

	if (m_Covercomponent->IsCover()) return;
	if (!m_Covercomponent->GetCoverWall()) return;

	m_Covercomponent->StopCover();
}

void UPlayerInputComponent::StartReload()
{
	OnCombatWidgetVisible.Broadcast(true);
	//m_PlayerWeapon->StartReload();
	if(!m_PlayerWeapon->CanReload()) return;
	m_inputData.IsReload = true;
}

void UPlayerInputComponent::VisibleHud()
{
	OnAllWidgetVisible.Broadcast(true);

}

void UPlayerInputComponent::HPreduce()
{
	owner->FindComponentByClass<UPlayerStatComponent>()->Attacked(100.0f);
}

void UPlayerInputComponent::HPregen()
{
	owner->FindComponentByClass<UStatComponent>()->RecoverHP(100000.0f);
}

void UPlayerInputComponent::GamePause()
{
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		OnCreatePauseWidget.ExecuteIfBound();
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}
