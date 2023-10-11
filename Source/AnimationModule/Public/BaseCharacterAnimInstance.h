// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATIONMODULE_API UBaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UBaseCharacterAnimInstance();
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "GunIKVec"), Category = "HandIK")
	FVector m_GunIKVec;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "IsHandIK"), Category = "HandIK")
	bool m_IsHandIK;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "IsFaceRight"))
	bool m_IsFaceRight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "IsReloding"))
	bool m_IsReloding;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "IsFire"))
	bool m_IsFire;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "IsAiming"))
	bool m_IsAiming;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "IsPakuru"))
	bool m_IsPakuru;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "owner"))
	TObjectPtr<class ACharacter> m_Owner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Weapon"))
	TObjectPtr<class UWeaponComponent> m_Weapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Input"))
	TObjectPtr<class UBaseInputComponent> m_Input;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Stats"))
	TObjectPtr<class UStatComponent> m_Stats;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Cover"))
	TObjectPtr<class UCoverComponent> m_Cover;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Pakur"))
	TObjectPtr<class UActorComponent> m_PakurComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Movement"))
	TObjectPtr<class UBaseCharacterMovementComponent> m_Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "HandGrip_R"))
	FName m_HandGrip_R;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "HandGrip_L"))
	FName m_HandGrip_L;
};
