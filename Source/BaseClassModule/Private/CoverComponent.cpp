// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "BaseInputComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>

//#include "PlayerMoveComponent.h"
//#include "WeaponComponent.h"

// Sets default values for this component's properties
UCoverComponent::UCoverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCoverComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = Cast<ACharacter>(GetOwner());
	m_IsCover = false;

}

// Called every frame
void UCoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RotateSet();
	TurnCheck(DeltaTime);
	if (m_TurnTime > 1.0f) {

		m_TurnTime = 0.0f;
	}

}

void UCoverComponent::PlayCover()
{
	if (m_IsCover) {
		StopCover();
		return;
	}
	
	FHitResult result;
	FVector start = owner->GetActorLocation();
	FVector end = start + (owner->GetActorForwardVector() * owner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2);
	FCollisionQueryParams params(NAME_None, true, owner);
	
	if (GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, params)) {
		StartCover(result);
		DrawDebugLine(GetWorld(), start, start + result.Normal * 200, FColor::Blue, false, 100.f);
	}

}

void UCoverComponent::SettingMoveVector(FVector& vector)
{
	if (!m_IsCover) return;


	float value = owner->GetActorRightVector().Dot(vector) > 0 ? 1 : -1;

	m_CoverLimit = value;
	FHitResult result = CheckCoverCollision();
	if (result.bBlockingHit) {
		vector = m_CoverLimit * owner->GetActorRightVector();
	}
	else {
		vector = FVector::ZeroVector;
		m_IsTurnWait = true;
	}
}

void UCoverComponent::TurnCheck(float DeltaTime)
{
	if (m_IsTurnWait) {
		m_IsTurnWait = false;
		m_TurnTime += DeltaTime;
	}
	else {
		m_TurnTime = 0.0f;
	}
}

void UCoverComponent::RotateSet()
{
	if (!m_IsCover) return;
	UCharacterMovementComponent* movement = owner->GetCharacterMovement();

	FHitResult result;
	FVector start = owner->GetActorLocation();
	FVector end = start + (-movement->GetPlaneConstraintNormal() * 1002);

	owner->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, end));
}


void UCoverComponent::StartCover(FHitResult& reslut)
{
	UCharacterMovementComponent* movement = owner->GetCharacterMovement();
	

	movement->SetPlaneConstraintEnabled(true);
	movement->SetPlaneConstraintNormal(reslut.Normal);

	owner->SetActorLocation(reslut.Location + reslut.Normal * owner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.01f);

	m_IsCover = true;
}

void UCoverComponent::StopCover()
{
	UCharacterMovementComponent* movement = owner->GetCharacterMovement();
	movement->SetPlaneConstraintEnabled(false);

	m_IsCover = false;

}

FHitResult UCoverComponent::CheckCoverCollision()
{
	FHitResult result;
	FVector start = owner->GetActorLocation() + m_CoverLimit * owner->GetActorRightVector() * owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector end = start + (owner->GetActorForwardVector() * owner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2);
	FCollisionQueryParams params(NAME_None, true, owner);

	//DrawDebugLine(GetWorld(), start, end, FColor::Blue);
	GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, params);
	return result;
}
