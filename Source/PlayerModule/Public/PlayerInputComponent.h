// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInputComponent.h"
#include "PlayerInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UPlayerInputComponent : public UBaseInputComponent
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FChangedWeapon);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCombatWidgetVisible, bool);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAllWidgetVisible, bool);
	DECLARE_DELEGATE(FOnCreatePauseWidget);

public:
	FChangedWeapon OnChangedWeapon;
	FOnCombatWidgetVisible OnCombatWidgetVisible;
	FOnAllWidgetVisible OnAllWidgetVisible;
	FOnCreatePauseWidget OnCreatePauseWidget;


public:
	virtual void BeginPlay() override;
	void StartReload();

	UFUNCTION()
	void InputDie();

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void InputMove();
	void Crouching();
	void Runing();
	void StartFire();
	void StopFire();
	void StartAiming();
	void StopAiming();
	void ChangeMainWeapon();
	void ChangeSubWeapon();
	void StopCover();

	void VisibleHud();
	void HPreduce();
	void HPregen();
	void GamePause();

private:
	TObjectPtr<class UPlayerWeaponComponent> m_PlayerWeapon;
	TObjectPtr<class UPathFollowingComponent> m_PathFollowingComp;
	TObjectPtr<class UCoverComponent> m_Covercomponent;
	TObjectPtr<class UPlayerMoveComponent> m_PlayerMove;
	TObjectPtr<class UInputComponent> m_InputComponent;
	TObjectPtr<class UBaseCharacterMovementComponent> m_Movement;

};
