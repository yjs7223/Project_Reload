// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInputComponent.h"
#include "GameFramework/Character.h"
#include "PlayerWeaponComponent.h"
#include "WeaponComponent.h"
#include "PlayerStatComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include "PlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "PlayerHUDWidget.h"
#include "Player_HP_Widget.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "PlayerMoveComponent.h"
#include "CoverComponent.h"


void UPlayerInputComponent::BeginPlay()
{
	m_PlayerMove = GetOwner()->FindComponentByClass<UPlayerMoveComponent>();

	Super::BeginPlay();
}

void UPlayerInputComponent::BindInput()
{
	Super::BindInput();

	BindAxis("Move Forward / Backward", this, &UPlayerInputComponent::MoveForward);
	BindAxis("Move Right / Left", this, &UPlayerInputComponent::MoveRight);
	BindAction("Move", IE_Pressed, this, &UPlayerInputComponent::InputMove);

	BindAxis("Turn Right / Left Mouse", owner, &ACharacter::AddControllerYawInput);
	BindAxis("Turn Right / Left Mouse", m_PlayerMove.Get(), &UPlayerMoveComponent::truning);
	BindAxis("Look Up / Down Mouse", owner, &ACharacter::AddControllerPitchInput);

	BindAction("Crouch", IE_Pressed, this, &UPlayerInputComponent::Crouching);
	BindAction("Runing", IE_Pressed, this, &UPlayerInputComponent::Runing);

	BindAction("Fire", IE_Pressed, this, &UPlayerInputComponent::StartFire);
	BindAction("Fire", IE_Released, this, &UPlayerInputComponent::StopFire);

	BindAction("Aim", IE_Pressed, this, &UPlayerInputComponent::StartAiming);
	BindAction("Aim", IE_Released, this, &UPlayerInputComponent::StopAiming);

	BindAction("Reload", IE_Pressed, this, &UPlayerInputComponent::StartReload);
	BindAction("Reload", IE_Released, this, &UPlayerInputComponent::StopReload);

	BindAction("ChangeMainWeapon", IE_Pressed, this, &UPlayerInputComponent::ChangeMainWeapon);
	BindAction("ChangeSubWeapon", IE_Pressed, this, &UPlayerInputComponent::ChangeSubWeapon);

	BindAction("Cover", IE_Pressed, this, &UPlayerInputComponent::StartCover);
	BindAction("Cover", IE_Released, this, &UPlayerInputComponent::StopCover);
	BindAction("Aim", IE_Pressed, m_Covercomponent.Get(), &UCoverComponent::StartPeeking);
	BindAction("Aim", IE_Released, m_Covercomponent.Get(), &UCoverComponent::StopPeeking);

	BindAction("UI_Visible", IE_Pressed, this, &UPlayerInputComponent::VisibleHud);
	BindAction("GamePause", IE_Pressed, this, &UPlayerInputComponent::GamePause);

	UPlayerStatComponent* statcomp = GetOwner()->FindComponentByClass<UPlayerStatComponent>();
	BindAction("Interactive", IE_Pressed, statcomp, &UPlayerStatComponent::Interacting);

	BindAction("HP_reduce", IE_Pressed, this, &UPlayerInputComponent::HPreduce);
	BindAction("HP_regen", IE_Pressed, this, &UPlayerInputComponent::HPregen);
}

void UPlayerInputComponent::SetCrowdControl(bool IsControl)
{
	Super::SetCrowdControl(IsControl);

	if (IsControl) {
		StopFire();
		StopAiming();
		StopReload();
		StopFire();
		m_inputData.movevec = FVector::ZeroVector;

	}
}

void UPlayerInputComponent::InputMove()
{
	if (!m_Covercomponent->IsCover() && m_PathFollowingComp->GetStatus() == EPathFollowingStatus::Moving) {
		m_Covercomponent->StopCover();
	}
}

void UPlayerInputComponent::ChangeMainWeapon()
{
	if (m_Weapon->weapontype == EWeaponType::Rifle) return;
	m_Weapon->weapontype = EWeaponType::Rifle;
	OnChangedWeapon.Broadcast();
}
	
void UPlayerInputComponent::ChangeSubWeapon()
{
	if (m_Weapon->weapontype == EWeaponType::Pistol) return;
	m_Weapon->weapontype = EWeaponType::Pistol;
	OnChangedWeapon.Broadcast();
}

void UPlayerInputComponent::StartCover()
{
	if (bIsbCrowdControl) return;
	m_Covercomponent->PlayCover();
}

void UPlayerInputComponent::StopCover()
{
	if (!m_Covercomponent->IsNextCover()) return;

	m_Covercomponent->StopCover();
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
