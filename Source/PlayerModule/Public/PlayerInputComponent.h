// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInputComponent.h"
#include "PlayerInputComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), hidecategories = (Activation, "Components|Activation"))
class PLAYERMODULE_API UPlayerInputComponent : public UBaseInputComponent
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FChangedWeapon);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAllWidgetVisible, bool);
	DECLARE_DELEGATE(FOnCreatePauseWidget);

public:
	FChangedWeapon OnChangedWeapon;
	FOnAllWidgetVisible OnAllWidgetVisible;
	FOnCreatePauseWidget OnCreatePauseWidget;

protected:
	virtual void BeginPlay() override;
	virtual void BindInput() override;

public
	void StartReload();

	UFUNCTION()
	void InputDie();

private:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void InputMove();
	virtual void StartReload() override;

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

protected:
	TObjectPtr<class UPlayerMoveComponent> m_PlayerMove;
};
