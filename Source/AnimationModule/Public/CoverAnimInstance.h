// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EPeekingState.h"
#include "ECoverShootingState.h"
#include "Engine/DataTable.h"
#include "CoverAnimInstance.generated.h"
/**
 *
 */
USTRUCT(Atomic, BlueprintType)
struct FCoverAnimationTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	bool IsVaild();

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* StartCover_LowToLow_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* StartCover_HighToHigh_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* StartCover_HighToLow_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* StartCover_LowToLow_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* StartCover_HighToHigh_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* StartCover_HighToLow_Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* EndCover_LowToLow_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* EndCover_LowToLow_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* EndCover_HighToHigh_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* EndCover_HighToHigh_Left;



};
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
	void AnimationSetting();

	void OutCoverEnd(UAnimMontage* Montage, bool bInterrupted);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
	class UBaseInputComponent* m_Input;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
	class UCoverComponent* mCover;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
	class UWeaponComponent* mWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = animation)
	class USkeletalMeshComponent* mWeaponMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "PakurComp"))
	TObjectPtr<class UActorComponent> m_PakurComp;
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation, meta = (DisplayName = "IsWeaponReload"))
	bool m_IsWeaponReload;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
	bool mIsCoverShooting;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
	bool mIsConerWait;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation, meta = (DisplayName = "CanShooting"))
	bool m_CanShooting;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation, meta = (DisplayName = "IsUsingWeapon"))
	bool m_IsUsingWeapon;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDataTable* m_AnimationTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	struct FCoverAnimationTable m_CurrentAnimation;
};
