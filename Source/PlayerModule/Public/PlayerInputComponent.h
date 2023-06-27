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
protected:
	virtual void BeginPlay() override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Crouching();
	void Runing();
	void StartFire();
	void StopFire();
	void StartAiming();
	void StopAiming();
	void StartReload();
	void CoverInputEvent();
	void StartPeeking();
	void StopPeeking();
};
