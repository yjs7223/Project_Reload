// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Engine/DataTable.h"
#include "WeaponAnimInstance.generated.h"


/**
 *
 */
USTRUCT(Atomic, BlueprintType)
struct FWeaponAnimationTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	bool IsVaild();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Aiming;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Aiming_Shooting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Shooting_Base;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* Shooting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Reload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* EquipWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* UnEquipWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAimOffsetBlendSpace* AimOffset;
};

/**
 * 
 */
UCLASS(ClassGroup = (Custom))
class ANIMATIONMODULE_API UWeaponAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UWeaponAnimInstance();

	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void AnimationSetting();

	UFUNCTION(BlueprintCallable)
	void PlayEquipMontage();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class ACharacter> m_Owner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UBaseInputComponent> m_Input;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UWeaponComponent> mWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UBaseCharacterMovementComponent> m_Movement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCoverComponent> m_Cover;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float mAimYaw;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float mAimPitch;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool mIsAiming;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool mIsFire;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool mIsFireing;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool mIsReload;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool mIsRuning;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "CanShooting"))
	bool m_CanShooting;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "IsUsingWeapon"))
	bool m_IsUsingWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "UpperMirror"))
	bool m_UpperMirror;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "UseUnderBody"))
	bool m_UseUnderBody;



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDataTable* m_AnimationTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	struct FWeaponAnimationTable m_CurrentAnimation;




};
