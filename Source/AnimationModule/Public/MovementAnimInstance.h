// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Engine/DataTable.h"
#include "MovementAnimInstance.generated.h"


/**
 *
 */
USTRUCT(Atomic, BlueprintType)
struct FMovementAimationTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	bool IsVaild();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Idle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Idle_Crouch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UBlendSpace* JogStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* JogStop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UBlendSpace* Move;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Run;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* SprintStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UBlendSpace* Move_Crouch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Falling;

};

/**
 * 
 */
UCLASS()
class ANIMATIONMODULE_API UMovementAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UMovementAnimInstance();

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void AnimationSetting();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		class ACharacter* owner;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		class UBaseInputComponent* m_Input;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		class UBaseCharacterMovementComponent* m_Movement;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		float mMoveSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		float mDirection;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		bool mIsMoving;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		bool mIsCrouching;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		bool mIsRuning;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* m_AnimationTable;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		struct FMovementAimationTable m_CurrentAnimation;

};
