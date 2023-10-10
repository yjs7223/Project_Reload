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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, DisplayName = "GunIKVec"), Category = "HandIK")
	FVector m_GunIKVec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, DisplayName = "IsHandIK"), Category = "HandIK")
	bool m_IsHandIK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, DisplayName = "IsFaceRight"))
	bool m_IsFaceRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, DisplayName = "IsReloding"))
	bool m_IsReloding;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, DisplayName = "IsFire"))
	bool m_IsFire;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, DisplayName = "IsAiming"))
	bool m_IsAiming;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, DisplayName = "IsPakuru"))
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, DisplayName = "HandGrip_R"))
	FName m_HandGrip_R;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, DisplayName = "HandGrip_L"))
	FName m_HandGrip_L;
};
