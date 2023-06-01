// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementAnimInstance.h"
#include "GameFramework/Character.h"
#include "PlayerMoveComponent.h"
#include "KismetAnimationLibrary.h"


UMovementAnimInstance::UMovementAnimInstance()
{
}

void UMovementAnimInstance::NativeBeginPlay()
{
	mPlayerMove = dynamic_cast<UPlayerMoveComponent*>(TryGetPawnOwner()->
		GetComponentByClass(UPlayerMoveComponent::StaticClass()));
}

void UMovementAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (ACharacter* charcter = dynamic_cast<ACharacter*>(TryGetPawnOwner())) {
		FVector tempVelocity = charcter->GetVelocity();
		mMoveSpeed = tempVelocity.Length();
		mDirection = UKismetAnimationLibrary::CalculateDirection(tempVelocity, charcter->GetActorRotation());

		mIsMoving = mMoveSpeed > 0;
		mIsCrouching = charcter->bIsCrouched;
	}
	if (mPlayerMove) {
		//mIsRuning = mPlayerMove->IsRuning();
	}
}


;