// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "StatsAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATIONMODULE_API UStatsAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UStatsAnimInstance();

	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = animation)
		class UStatComponent* mStats;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = animation)
		bool mIsDie;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = animation)
		bool mIsHit;
	
};
