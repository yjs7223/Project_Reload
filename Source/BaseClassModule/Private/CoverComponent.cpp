// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "BaseInputComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include "PlayerMovable.h"
#include "WeaponComponent.h"


#include "Blueprint/AIBlueprintHelperLibrary.h"

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

	m_Movement = owner->GetCharacterMovement();
	m_Inputdata = owner->FindComponentByClass<UBaseInputComponent>()->getInput();
	m_Weapon = owner->FindComponentByClass<UWeaponComponent>();
}

// Called every frame
void UCoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!m_IsCover) return;
	m_Inputdata->IsRuning = false;

	RotateSet();
	TurnCheck(DeltaTime);

	if (m_IsCornering) {

		if (owner->GetVelocity().Length() == 0) {
			StopCornering(DeltaTime);
		}
		else {
			PlayingCornering(DeltaTime);
		}
	}

	if (m_TurnTime > 1.0f) {
		m_TurnTime = 0.0f;
		PlayCornering();
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
	}

}

void UCoverComponent::SettingMoveVector(FVector& vector)
{
	if (!m_IsCover) return;


	float value = owner->GetActorRightVector().Dot(vector) > 0 ? 1 : -1;

	m_FaceRight = value;
	FHitResult result = CheckCoverCollision();
	if (result.bBlockingHit) {
		vector = m_FaceRight * owner->GetActorRightVector();
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

void UCoverComponent::AimSetting(FRotator& aimOffset)
{
	if (!m_IsCover) return;
	//if (isPeeking()) {
	//	if (!mIsFaceRight) mAimYaw *= -1.0f;
	//	mWeapon->mAimYaw = mAimYaw;
	//	return;
	//}

	if (aimOffset.Yaw > 0) {
		if (m_Inputdata->IsAiming || (m_Inputdata->IsFire && aimOffset.Yaw > 45)) {
			aimOffset.Yaw -= 180;
			m_FaceRight = 1.0f;
		}
	}
	else {
		if (m_Inputdata->IsAiming || (m_Inputdata->IsFire && aimOffset.Yaw < -45)) {
			aimOffset.Yaw += 180;
			aimOffset.Yaw *= -1.0f;
			m_FaceRight = -1.0f;
		}
	}
}

bool UCoverComponent::RotateSet()
{
	if (!m_IsCover) return false;

	FHitResult result;
	FVector start = owner->GetActorLocation();
	FVector end = start + (owner->GetActorForwardVector() * owner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2);
	FCollisionQueryParams params(NAME_None, true, owner);

	GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, params);
	
	m_Movement->SetPlaneConstraintNormal(result.Normal);

	FVector target = start + (-result.Normal);

	owner->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
	return true;
}

bool UCoverComponent::IsCover()
{
	return m_IsCover;
}

bool UCoverComponent::IsTurnWait()
{
	return m_IsTurnWait;
}

float UCoverComponent::FaceRight()
{
	return m_FaceRight;
}

bool UCoverComponent::IsCornering()
{
	return m_IsCornering;
}


void UCoverComponent::StartCover(FHitResult& reslut)
{
	RotateSet();
	m_Movement->SetPlaneConstraintEnabled(true);
	owner->SetActorLocation(reslut.Location + reslut.Normal * owner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.01f);
	m_IsCover = true;
}

void UCoverComponent::StopCover()
{
	m_Movement->SetPlaneConstraintEnabled(false);
	m_IsCover = false;
}

FHitResult UCoverComponent::CheckCoverCollision()
{
	FHitResult result;
	FVector start = owner->GetActorLocation() + m_FaceRight * owner->GetActorRightVector() * owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector end = start + (owner->GetActorForwardVector() * owner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2);
	FCollisionQueryParams params(NAME_None, true, owner);

	//DrawDebugLine(GetWorld(), start, end, FColor::Blue);
	GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, params);
	return result;
}

void UCoverComponent::PlayCornering()
{
	FHitResult result1 = CheckCoverCollision();

	FHitResult result2;
	FVector start = result1.TraceEnd;
	FVector end = start + -m_FaceRight * owner->GetActorRightVector() * owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
	FCollisionQueryParams params(NAME_None, true, owner);

	GetWorld()->LineTraceSingleByChannel(result2, start, end, ECC_Visibility, params);

	if (!result2.bBlockingHit) return;

	FVector targetPoint = result2.Location + owner->GetActorForwardVector() * owner->GetCapsuleComponent()->GetScaledCapsuleRadius() + result2.Normal * owner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.01f;
	DrawDebugSphere(GetWorld(), targetPoint, 10.0f, 32, FColor::Magenta, false, 20.0f);
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(owner->GetController(), targetPoint);

	for (auto& item : owner->GetComponentsByInterface(UPlayerMovable::StaticClass()))
	{
		Cast<IPlayerMovable>(item)->SetCanMove(false);
	}

	m_Movement->SetPlaneConstraintEnabled(false);
	m_IsCornering = true;
	m_Turnlookpoint = targetPoint - result2.Normal * owner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2;

	owner->GetMovementComponent()->AddInputVector( -m_FaceRight * owner->GetActorRightVector());
}



void UCoverComponent::StopCornering(float DeltaTim)
{
	for (auto& item : owner->GetComponentsByInterface(UPlayerMovable::StaticClass()))
	{
		Cast<IPlayerMovable>(item)->SetCanMove(true);
	}
	
	m_Movement->SetPlaneConstraintEnabled(true);
	m_IsCornering = false;
	m_Turnlookpoint = FVector::ZeroVector;
}

void UCoverComponent::PlayingCornering(float DeltaTim)
{
	owner->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(owner->GetActorLocation(), m_Turnlookpoint));
		
}
