// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MovementAnimInstance.generated.h"

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


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = animation)
		class UBaseInputComponent* m_Input;

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
	
};
