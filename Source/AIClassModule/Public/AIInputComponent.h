// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInputComponent.h"
#include "AIInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API UAIInputComponent : public UBaseInputComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;


public:
	UFUNCTION(BlueprintCallable, Category = "Anim")
	void AIMoveForward(float Value);
	UFUNCTION(BlueprintCallable, Category = "Anim")
	void AIMoveRight(float Value);
	UFUNCTION(BlueprintCallable, Category = "Anim")
	void AICrouching();
	UFUNCTION(BlueprintCallable, Category = "Anim")
	void AIRuning();
	UFUNCTION(BlueprintCallable, Category = "Anim")
	void AIStartFire();
	UFUNCTION(BlueprintCallable, Category = "Anim")
	void AIStopFire();
	UFUNCTION(BlueprintCallable, Category = "Anim")
	void AIStartAiming();
	UFUNCTION(BlueprintCallable, Category = "Anim")
	void AIStopAiming();
	UFUNCTION(BlueprintCallable, Category = "Anim")
	void AIStartReload();
};
