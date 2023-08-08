// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterMovementComponent.h"

bool UBaseCharacterMovementComponent::isRuning() const
{
	return CustomMovementMode == CMOVE_Runing;
}

bool UBaseCharacterMovementComponent::CheckFall(const FFindFloorResult& OldFloor, const FHitResult& Hit, const FVector& Delta, const FVector& OldLocation, float remainingTime, float timeTick, int32 Iterations, bool bMustJump)
{
	if (!Super::CheckFall(OldFloor, Hit, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump)) return false;
	// 상위에서 트루 반환후 런상태 떨어지기 체크를 합니다
	if (isRuning()) {
		SetMovementMode(MOVE_Walking);

		StartFalling(Iterations, remainingTime, timeTick, Delta, OldLocation);
	}
	return true;
}

float UBaseCharacterMovementComponent::GetMaxSpeed() const
{
	switch (CustomMovementMode)
	{
	case CMOVE_Runing:
		return MaxRuningSpeed;
		break;
	default:
		break;
	}
	return Super::GetMaxSpeed();
}

void UBaseCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
	case CMOVE_Runing:
		PhysWalking(deltaTime, Iterations);

		break;
	default:
		break;
	}
}
