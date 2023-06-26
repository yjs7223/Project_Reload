// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementAnimInstance.h"
#include "GameFramework/Character.h"
#include "PlayerMoveComponent.h"
#include "KismetAnimationLibrary.h"
#include "BaseInputComponent.h"


UMovementAnimInstance::UMovementAnimInstance()
{
}

void UMovementAnimInstance::NativeBeginPlay()
{
	m_Input = dynamic_cast<UBaseInputComponent*>(TryGetPawnOwner()->FindComponentByClass<UBaseInputComponent>());

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
	if (m_Input) {
		mIsRuning =m_Input->getInput()->IsRuning;
	}
}


;