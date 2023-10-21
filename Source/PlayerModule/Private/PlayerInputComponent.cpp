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
#include "PlayerMoveComponent.h"


void UPlayerInputComponent::BeginPlay()
{
	Super::BeginPlay();
	m_PlayerWeapon = owner->FindComponentByClass<UPlayerWeaponComponent>();
	m_Covercomponent = owner->FindComponentByClass<UCoverComponent>();
	m_PlayerMove = owner->FindComponentByClass<UPlayerMoveComponent>();
	m_InputComponent = owner->InputComponent;
	m_PathFollowingComp = owner->GetController()->FindComponentByClass<UPathFollowingComponent>();
	m_Movement = owner->FindComponentByClass<UBaseCharacterMovementComponent>();

	m_InputComponent->BindAxis("Move Forward / Backward", this, &UPlayerInputComponent::MoveForward);
	m_InputComponent->BindAxis("Move Right / Left", this, &UPlayerInputComponent::MoveRight);
	m_InputComponent->BindAction("Move", IE_Pressed, this, &UPlayerInputComponent::InputMove);

	m_InputComponent->BindAxis("Turn Right / Left Mouse", owner, &ACharacter::AddControllerYawInput);
	m_InputComponent->BindAxis("Turn Right / Left Mouse", m_PlayerMove.Get(), &UPlayerMoveComponent::truning);
	m_InputComponent->BindAxis("Look Up / Down Mouse", owner, &ACharacter::AddControllerPitchInput);

	m_InputComponent->BindAction("Crouch", IE_Pressed, this, &UPlayerInputComponent::Crouching);
	m_InputComponent->BindAction("Runing", IE_Pressed, this, &UPlayerInputComponent::Runing);

	m_InputComponent->BindAction("Fire", IE_Pressed, this, &UPlayerInputComponent::StartFire);
	m_InputComponent->BindAction("Fire", IE_Released, this, &UPlayerInputComponent::StopFire);

	m_InputComponent->BindAction("Aim", IE_Pressed, this, &UPlayerInputComponent::StartAiming);
	m_InputComponent->BindAction("Aim", IE_Released, this, &UPlayerInputComponent::StopAiming);

	m_InputComponent->BindAction("Reload", IE_Pressed, this, &UPlayerInputComponent::StartReload);

	m_InputComponent->BindAction("ChangeMainWeapon", IE_Pressed, this, &UPlayerInputComponent::ChangeMainWeapon);
	m_InputComponent->BindAction("ChangeSubWeapon", IE_Pressed, this, &UPlayerInputComponent::ChangeSubWeapon);

	m_InputComponent->BindAction("Cover", IE_Pressed, m_Covercomponent.Get(), &UCoverComponent::PlayCover);
	m_InputComponent->BindAction("Cover", IE_Released, this, &UPlayerInputComponent::StopCover);
	m_InputComponent->BindAction("Aim", IE_Pressed, m_Covercomponent.Get(), &UCoverComponent::StartPeeking);
	m_InputComponent->BindAction("Aim", IE_Released, m_Covercomponent.Get(), &UCoverComponent::StopPeeking);

	m_InputComponent->BindAction("HP_reduce", IE_Pressed, this, &UPlayerInputComponent::HPreduce);
	m_InputComponent->BindAction("HP_regen", IE_Pressed, this, &UPlayerInputComponent::HPregen);

	//APlayerCharacter* pc = Cast<APlayerCharacter>(owner);
	m_InputComponent->BindAction("UI_Visible", IE_Pressed, this, &UPlayerInputComponent::VisibleHud);
	m_InputComponent->BindAction("GamePause", IE_Pressed, this, &UPlayerInputComponent::GamePause);

	UPlayerStatComponent* statcomp = GetOwner()->FindComponentByClass<UPlayerStatComponent>();
	m_InputComponent->BindAction("Interactive", IE_Pressed, statcomp, &UPlayerStatComponent::Interacting);

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
	if (m_Covercomponent->IsCover()) return;
	if (m_inputData.IsReload) return;

	if (m_Movement->isRuning()) {
		m_Movement->SetMovementMode(MOVE_Walking);
	}
	else {
		m_Movement->SetMovementMode(MOVE_Custom, CMOVE_Runing);
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

	m_inputData.IsAiming = true;
	if (m_Movement->isRuning()) {
		m_Movement->SetMovementMode(MOVE_Walking);
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
	if (m_PlayerWeapon->weapontype == EWeaponType::Rifle) return;
	m_PlayerWeapon->weapontype = EWeaponType::Rifle;
	OnChangedWeapon.Broadcast();
}
	
void UPlayerInputComponent::ChangeSubWeapon()
{
	if (m_PlayerWeapon->weapontype == EWeaponType::Pistol) return;
	m_PlayerWeapon->weapontype = EWeaponType::Pistol;
	OnChangedWeapon.Broadcast();
}

void UPlayerInputComponent::StopCover()
{
	UBaseCharacterMovementComponent* movement = Cast<UBaseCharacterMovementComponent>(owner->GetCharacterMovement());

	if (!m_Covercomponent->IsNextCover()) return;


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
