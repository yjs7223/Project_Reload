// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMoveComponent.h"
#include "PlayerCharacter.h"
#include "PlayerWeaponComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Math/Axis.h"

// Sets default values for this component's properties
UPlayerMoveComponent::UPlayerMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner<APlayerCharacter>();
	moveSpeed = 100.0f;
	turnSpeed = 0.05f;
	// ...
	
}


// Called every frame
void UPlayerMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Moving(DeltaTime);
	Turning(DeltaTime);
	// ...
}

void UPlayerMoveComponent::bindInput(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveForward", this, &UPlayerMoveComponent::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &UPlayerMoveComponent::MoveRight);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &UPlayerMoveComponent::StartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &UPlayerMoveComponent::StopRun);

	if (!owner) { owner = GetOwner<APlayerCharacter>(); }
	PlayerInputComponent->BindAxis("Turn", owner, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", owner, &APlayerCharacter::AddControllerPitchInput);
}

void UPlayerMoveComponent::MoveForward(float value)
{
	moveVec.X = value;
}

void UPlayerMoveComponent::MoveRight(float value)
{
	moveVec.Y = value;
}

void UPlayerMoveComponent::Moving(float DeltaTime)
{
	if (moveVec == FVector::ZeroVector) {
		isMove = false;
		return;
	}
	else
	{
		isMove = true;
	}
	FVector tempActorpos = owner->GetActorLocation();

	tempActorpos.Z = 0;
	moveVec.Normalize();

	FVector MoveDirect = owner->Controller->GetControlRotation().RotateVector(moveVec);
	FRotator targetRotate;
	MoveDirect.Z = 0;
	MoveDirect.Normalize();

	targetRotate = owner->GetActorRotation();
	targetRotate.Yaw = owner->Controller->GetControlRotation().Yaw;

	mTargetRotate = targetRotate;
	mMoveDirect = FMath::VInterpTo(mMoveDirect, MoveDirect, DeltaTime, 8.f);
	float m_speed = moveSpeed;
	if (isRun)
	{
		m_speed = m_speed * 2.5f;
	}
	owner->GetMovementComponent()->AddInputVector(mMoveDirect * m_speed);
}

void UPlayerMoveComponent::Turning(float DeltaTime)
{
	if (owner->weapon->isAiming)
	{
		FRotator mrot = owner->GetActorRotation();
		mrot.Yaw = owner->FollowCamera->GetComponentRotation().Yaw;
	}
	else
	{
		owner->SetActorRelativeRotation(FMath::RInterpTo(owner->GetActorRotation(), mTargetRotate, DeltaTime, turnSpeed));
		float yawValue = FMath::Abs((owner->GetActorRotation() - mTargetRotate).Yaw);
		owner->SetActorRelativeRotation(mTargetRotate);
		/*if (yawValue < 5 || yawValue > 355) {
			owner->SetActorRelativeRotation(mTargetRotate);
		}*/
	}
}

void UPlayerMoveComponent::StartRun()
{
	isRun = true;
}

void UPlayerMoveComponent::StopRun()
{
	isRun = false;
}
