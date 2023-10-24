// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InputComponent.h"
#include "BaseInputComponent.generated.h"


USTRUCT(BlueprintType)
struct FInputData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Editor)
	FVector movevec;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Editor)
	uint8 IsFire : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Editor)
	uint8 IsAiming : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Editor)
	uint8 IsReload : 1;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BASECLASSMODULE_API UBaseInputComponent : public UInputComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	FInputData* getInput();

	bool m_CanUnCrouch;

	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);
	virtual void Crouching();
	virtual void Runing();
	virtual void StartFire();
	virtual void StopFire();
	virtual void StartAiming();
	virtual void StopAiming();
	virtual void StartReload();


protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Editor)
	ACharacter* owner;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Editor)
	FInputData m_inputData;

protected:
	TObjectPtr<class UWeaponComponent> m_Weapon;
	TObjectPtr<class UPathFollowingComponent> m_PathFollowingComp;
	TObjectPtr<class UCoverComponent> m_Covercomponent;
	TObjectPtr<class UInputComponent> m_InputComponent;
	TObjectPtr<class UBaseCharacterMovementComponent> m_Movement;
};
