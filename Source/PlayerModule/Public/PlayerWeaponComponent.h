// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponComponent.h"
#include "Input.h"
#include "PlayerWeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UPlayerWeaponComponent : public UWeaponComponent, public IInput
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UPlayerWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void bindInput(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void Fire() override;
};
