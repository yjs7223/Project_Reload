// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EPeekingState.h"
#include "ECoverShootingState.h"
#include "CoverAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATIONMODULE_API UCoverAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UCoverAnimInstance();

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		class UBaseInputComponent* m_Input;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		class UCoverComponent* mCover;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		class UWeaponComponent* mWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = animation)
		class USkeletalMeshComponent* mWeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = animation)
		FName mRightHandName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = animation)
		FName mLeftHandName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		bool mIsCover;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		bool mIsFaceRight;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		bool mIsCrouching;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		bool mIsMoving;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		bool mIsCornering;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		bool mIsAiming;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		bool mIsPeeking;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		bool mIsFire;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		float mAimYaw;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		float mAimPitch;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		bool mIsReload;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		bool mIsCoverShooting;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		bool mIsConerWait;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		EPeekingState  mPeekingState;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		EPeekingState  mLastPeekingState;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		ECoverShootingState  mCoverSootingState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		FRotator mSpinRotater;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		FVector mBeforPos;
	
};
