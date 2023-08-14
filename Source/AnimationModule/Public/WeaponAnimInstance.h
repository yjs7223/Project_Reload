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
	void PlayShootingAnimation();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		class UBaseInputComponent* m_Input;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = animation)
		class UWeaponComponent* mWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = animation)
	class UBaseCharacterMovementComponent* m_Movement;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		float mAimYaw;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		float mAimPitch;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		bool mIsAiming;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		bool mIsFire;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		bool mIsFireing;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		bool mIsReload;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		bool mIsRuning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDataTable* m_AnimationTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	struct FWeaponAnimationTable m_CurrentAnimation;




};
