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

UCLASS(NonTransient, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), hidecategories = (Activation, "Components|Activation"))
class BASECLASSMODULE_API UBaseInputComponent : public UInputComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void BindInput();
	virtual void UnBindInput();

	FInputData* getInput();


	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);
	virtual void Crouching();
	virtual void Runing();
	virtual void StartFire();
	virtual void StopFire();
	virtual void StartAiming();
	virtual void StopAiming();
	virtual void StartReload();
	virtual void StopReload();


public:
	bool getCanUnCrouch() { return m_CanUnCrouch;}
	bool getInputEnable() { return m_IsInputEnabled;}

	void SetCanUnCrouch(bool CanUnCrouch) { m_CanUnCrouch = CanUnCrouch;}
	void SetInputEnable(bool IsEnable);

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	ACharacter* owner;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FInputData m_inputData;

	bool m_CanUnCrouch;
	bool m_IsInputEnabled;


protected:
	TObjectPtr<class UWeaponComponent> m_Weapon;
	TObjectPtr<class UPathFollowingComponent> m_PathFollowingComp;
	TObjectPtr<class UCoverComponent> m_Covercomponent;
	TObjectPtr<class UInputComponent> m_InputComponent;
	TObjectPtr<class UBaseCharacterMovementComponent> m_Movement;
};
