// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMoveComponent.h"
#include "GameFramework/Character.h"
#include "BaseInputComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UPlayerMoveComponent::UPlayerMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	mCanUnCrouch = true;
	// ...
}


// Called when the game starts
void UPlayerMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	owner = dynamic_cast<ACharacter*>(GetOwner());
	ensure(owner);


	mCanMove = true;
}


// Called every frame
void UPlayerMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!mCanMove) return;
	Moving(DeltaTime);
	Turning(DeltaTime);
}

void UPlayerMoveComponent::Turning(float DeltaTime)
{
	owner->SetActorRelativeRotation(FMath::RInterpTo(owner->GetActorRotation(), mTargetRotate, DeltaTime, turningspped));
	float yawValue = FMath::Abs((owner->GetActorRotation() - mTargetRotate).Yaw);
	if (yawValue < 5 || yawValue > 355) {
		owner->SetActorRelativeRotation(mTargetRotate);
	}
}

void UPlayerMoveComponent::Turn()
{
	mTargetRotate.Yaw = owner->Controller->GetControlRotation().Yaw;
}

void UPlayerMoveComponent::Moving(float DeltaTime)
{
	FInputData* data = owner->FindComponentByClass<UBaseInputComponent>()->getInput();
	if (data->movevec == FVector::ZeroVector) {
		data->IsRuning = false;
		return;
	}

	FVector tempActorpos = owner->GetActorLocation();
	tempActorpos.Z = 0;
	data->movevec.Normalize();

	FVector MoveDirect = owner->Controller->GetControlRotation().RotateVector(data->movevec);
	FRotator targetRotate;
	MoveDirect.Z = 0;
	MoveDirect.Normalize();	

	targetRotate = owner->GetActorRotation();
	targetRotate.Yaw = owner->Controller->GetControlRotation().Yaw;
	

	if (data->IsRuning) {
		MoveDirect *= 2;
		targetRotate = MoveDirect.Rotation();

	}
	
	mTargetRotate = targetRotate;
	
	mMoveDirect = FMath::VInterpTo(mMoveDirect, MoveDirect, DeltaTime, 8.f);
	owner->GetMovementComponent()->AddInputVector(mMoveDirect * movespeed);

	

}

void UPlayerMoveComponent::SetCanMove(bool canmove)
{
	mCanMove = canmove;
}

void UPlayerMoveComponent::SetCanUnCrouch(bool canCrouch)
{
	mCanUnCrouch = canCrouch;
}

bool UPlayerMoveComponent::IsCanMove()
{
	return mCanMove;
}

