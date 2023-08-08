// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMoveComponent.h"
#include "GameFramework/Character.h"
#include "BaseInputComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CoverComponent.h"
#include "Pakurable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Navigation/PathFollowingComponent.h"
#include "BaseCharacterMovementComponent.h"


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

	owner = dynamic_cast<ACharacter*>(GetOwner());
	ensure(owner);

	owner->FindComponentByClass<UBaseInputComponent>()->m_CanUnCrouch = true;
	m_CoverComp = owner->FindComponentByClass<UCoverComponent>();
	m_Inputdata = owner->FindComponentByClass<UBaseInputComponent>()->getInput();
	m_PathFollowingComp = owner->GetController()->FindComponentByClass<UPathFollowingComponent>();
	m_Movement = Cast<UBaseCharacterMovementComponent>(owner->GetCharacterMovement());

	if (m_PathFollowingComp == nullptr) {
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]() {
			m_PathFollowingComp = owner->GetController()->FindComponentByClass<UPathFollowingComponent>();
		});
	}

	TArray<UActorComponent*> pakurArr = owner->GetComponentsByInterface(UPakurable::StaticClass());
	if (pakurArr.Num() > 0) {
		m_PakurComp = pakurArr[0];
	}

	mCanMove = true;
	//turningspped
}


// Called every frame
void UPlayerMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!mCanMove) return;


	if (m_PakurComp && IPakurable::Execute_IsRolling(m_PakurComp)) return;
	Moving(DeltaTime);
	Turning(DeltaTime);
}

void UPlayerMoveComponent::Turning(float DeltaTime)
{
	if (m_CoverComp && m_CoverComp->IsCover()) {
		
		return;
	}

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
	if (m_PathFollowingComp && m_PathFollowingComp->GetStatus() == EPathFollowingStatus::Moving) {
		mTargetRotate = owner->GetVelocity().Rotation();
		return;
	}
	if (m_Inputdata->movevec == FVector::ZeroVector) {
		mMoveDirect = FVector::ZeroVector;
		m_Movement->SetMovementMode(MOVE_Walking);
		m_Inputdata->IsRuning = false;

		return;
	}

	FVector MoveDirect;
		MoveDirect = owner->Controller->GetControlRotation().RotateVector(m_Inputdata->movevec);
	if (m_CoverComp) {
		m_CoverComp->SettingMoveVector(MoveDirect);
	}


	MoveDirect.Z = 0;
	MoveDirect.Normalize();
	MoveDirect *= m_Movement->GetMaxSpeed();
	FRotator targetRotate = FRotator(0.0f, owner->Controller->GetControlRotation().Yaw, 0.0f);

	if (m_Inputdata->IsRuning) {
		targetRotate = MoveDirect.Rotation();
	}
	else {
		MoveDirect *= 0.5;
	}
	
	mTargetRotate = targetRotate;

	owner->AddMovementInput(MoveDirect);
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


