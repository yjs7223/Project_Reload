// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementAnimInstance.h"
#include "GameFramework/Character.h"
#include "PlayerMoveComponent.h"
#include "KismetAnimationLibrary.h"
#include "BaseInputComponent.h"
#include "BaseCharacterMovementComponent.h"

#include "Kismet/KismetSystemLibrary.h"

UMovementAnimInstance::UMovementAnimInstance()
{
}

void UMovementAnimInstance::NativeBeginPlay()
{
	owner = Cast<ACharacter>(TryGetPawnOwner());
	m_Input = owner->FindComponentByClass<UBaseInputComponent>();
	m_Movement = owner->FindComponentByClass<UBaseCharacterMovementComponent>();
}

void UMovementAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (owner) {
		FVector tempVelocity = owner->GetVelocity();
		mMoveSpeed = tempVelocity.Length();
		mDirection = UKismetAnimationLibrary::CalculateDirection(tempVelocity, owner->GetActorRotation());

		mIsMoving = mMoveSpeed > 0;
		mIsCrouching = owner->bIsCrouched;
	}
	if (m_Movement) {
		mIsRuning = m_Movement->isRuning();
	}
}


;