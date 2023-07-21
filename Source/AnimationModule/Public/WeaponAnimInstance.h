// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponAnimInstance.generated.h"

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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		class UBaseInputComponent* m_Input;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = animation)
		class UWeaponComponent* mWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = animation)
		class UPlayerMoveComponent* mPlayerMove;
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

	UPROPERTY(EditAnywhere, Category = Montage)
		UAnimMontage* mFireMontage;
	UPROPERTY(EditAnywhere, Category = Montage)
		UAnimMontage* mReloadMontage;
	UPROPERTY(EditAnywhere, Category = Montage)
		UAnimMontage* mAimingMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = animation)
	class UWeaponAnimDataAsset* test;
private:
	UPROPERTY(VisibleAnywhere, Category = molu)
		float mfireTime;
	UPROPERTY(VisibleAnywhere, Category = molu)
		float mReloadingTime;
};
