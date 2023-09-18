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

public:
	FChangedWeapon OnChangedWeapon;

protected:
	virtual void BeginPlay() override;

public:
	void StartReload();

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

	void TestHud();
	void HPregen();

private:
	TObjectPtr<class UPlayerWeaponComponent> m_PlayerWeapon;
	TObjectPtr<class UPathFollowingComponent> m_PathFollowingComp;
	TObjectPtr<class UCoverComponent> m_Covercomponent;
};
