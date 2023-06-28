// Fill out your copyright notice in the Description page of Project Settings.
#include "StatsAnimInstance.h"
#include "GameFramework/Character.h"
#include "StatComponent.h"


UStatsAnimInstance::UStatsAnimInstance()
{
}

void UStatsAnimInstance::NativeBeginPlay()
{
	ACharacter* owner = Cast<ACharacter>(TryGetPawnOwner());
	mStats = owner->FindComponentByClass<UStatComponent>();
}

void UStatsAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (mStats) {
		mIsDie = mStats->isDie;
		//mIsHit = mStats->isHit;
	}
}
