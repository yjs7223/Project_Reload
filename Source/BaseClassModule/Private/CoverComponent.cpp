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
#include "EPeekingState.h"
#include "Camera/CameraComponent.h"


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
	capsule = owner->GetCapsuleComponent();
	UInputComponent* input = owner->FindComponentByClass<UInputComponent>();

	input->BindAction("Aim", IE_Pressed, this, &UCoverComponent::StartPeeking);
	input->BindAction("Aim", IE_Released, this, &UCoverComponent::StopPeeking);

}

// Called every frame
void UCoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TestFN(DeltaTime);

	if (!m_IsCover) return;
	m_Inputdata->IsRuning = false;

	RotateSet(DeltaTime);
	AimSetting(DeltaTime);
	TurnCheck(DeltaTime);
	BeCrouch(DeltaTime);

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

	CalculateCoverShoot();
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
	
	if (GetWorld()->LineTraceSingleByChannel(result, start, end, traceChanel, params)) {
		StartCover(result);
	}

}

void UCoverComponent::SettingMoveVector(FVector& vector)
{
	if (!m_IsCover) return;
	if (isPeeking()) {
		vector = FVector::ZeroVector;
		return;
	}

	float value = owner->GetActorRightVector().Dot(vector) > 0 ? 1 : -1;

	m_FaceRight = value;
	FHitResult result;
	CheckCoverCollision(result);
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

void UCoverComponent::AimSetting(float DeltaTime)
{
	FRotator& aimOffset = m_Weapon->aimOffset;

	if (!m_IsCover) return;
	if (isPeeking()) {
		if (m_FaceRight < 0) aimOffset.Yaw *= -1.0f;
		return;
	}

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

void UCoverComponent::RotateSet(float DeltaTime)
{
	if (!m_IsCover || m_IsCornering) return;

	FHitResult result;
	FHitResult result2;
	FVector start;
	FVector end;
	FCollisionQueryParams params(NAME_None, true, owner);

	FVector FinalNormal;
	FVector FinalLocation;

	start = owner->GetActorLocation();
	end = start + (owner->GetActorForwardVector() * capsule->GetScaledCapsuleRadius() * 2.0f);
	GetWorld()->LineTraceSingleByChannel(result, start, end, traceChanel, params);

	if (!result.bBlockingHit) return;

	start = owner->GetActorLocation() + m_FaceRight * owner->GetActorRightVector() * capsule->GetScaledCapsuleRadius();
	end = start + (owner->GetActorForwardVector() * capsule->GetScaledCapsuleRadius() * 2.0f);
	GetWorld()->LineTraceSingleByChannel(result2, start, end, traceChanel, params);


	if (result2.bBlockingHit) {
		FHitResult tempResult;
		start = owner->GetActorLocation();
		end = start + (owner->GetActorForwardVector() * capsule->GetScaledCapsuleRadius() * 2.0f);

		UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), start, end,
			capsule->GetScaledCapsuleRadius(),
			capsule->GetScaledCapsuleHalfHeight(),
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			{},
			EDrawDebugTrace::None,
			tempResult,
			true);

		FinalLocation = tempResult.Location;
		FinalNormal = tempResult.Normal;

		/*if ((result2.TraceEnd - result2.TraceStart).GetSafeNormal().Dot(result2.Normal) + 1.0 < 0.1) {
			FinalLocation = result2.Location;
			FinalNormal = result2.Normal;
		}
		else {
			FHitResult tempResult;
			start = owner->GetActorLocation();
			end = owner->GetActorLocation() + m_FaceRight * owner->GetActorRightVector() * 1.1 * owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
			GetWorld()->LineTraceSingleByChannel(tempResult, start, end, traceChanel, params);
			if (tempResult.bBlockingHit) {
				FinalLocation = tempResult.Location;
				FinalNormal = tempResult.Normal;
			}
			else {
				start = owner->GetActorLocation() + m_FaceRight * owner->GetActorRightVector() * 1.1 * owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
				end = start + (owner->GetActorForwardVector() * owner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2.0f);
				GetWorld()->LineTraceSingleByChannel(tempResult, start, end, traceChanel, params);

				FinalLocation = tempResult.Location;
				FinalNormal = tempResult.Normal;
			}
		}*/
	}
	else {
		FinalLocation = result.Location;
		FinalNormal = result.Normal;
	}
	if (FinalNormal == FVector::ZeroVector) {
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ERROR FinalNormal is Zero"), true,true, FColor::Red);
	}
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, -FinalNormal);
	owner->SetActorRotation(FMath::RInterpTo(owner->GetActorRotation(), TargetRotation, DeltaTime, 6.0f));

	if (owner->GetActorRotation().Vector().Dot(TargetRotation.Vector()) >= 0.999) {
		m_IsWillPosSetting = true;
		owner->SetActorRotation(TargetRotation);
		
	}

	if (m_IsWillPosSetting) {
		result;
		start = owner->GetActorLocation();
		end = start + (owner->GetActorForwardVector() * owner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 3.0f);
		DrawDebugLine(GetWorld(), start, end, FColor::Emerald);
		if (GetWorld()->LineTraceSingleByChannel(result, start, end, traceChanel, params)) {
		FVector targetPos = result.Location + FinalNormal * owner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.01f;
			owner->SetActorLocation(FMath::VInterpTo(owner->GetActorLocation(), targetPos, DeltaTime, 6.0f));
			if ((owner->GetActorLocation() - targetPos).SizeSquared() <= 1) {
				owner->SetActorLocation(targetPos);
				m_IsWillPosSetting = false;
			}
		}
	}

	return;
}

void UCoverComponent::TestFN(float DeltaTime)
{
	TArray<FHitResult> results;
	FVector ViewPoint;
	FVector ViewVector;
	FRotator cameraRotation;
	owner->Controller->GetPlayerViewPoint(ViewPoint, cameraRotation);
	UCameraComponent* camera = owner->FindComponentByClass<UCameraComponent>();

	ViewVector = cameraRotation.Vector();
	if(!UKismetSystemLibrary::BoxTraceMulti(GetWorld(),
		ViewPoint + ViewVector * 200,
		ViewPoint + ViewVector * 1000,
		{  0, camera->OrthoWidth * 0.2f, camera->OrthoWidth* 0.27f},
		cameraRotation,
		UEngineTypes::ConvertToTraceType(traceChanel),
		true,
		{},
		EDrawDebugTrace::None,
		results,
		true
		)) return;

	DrawDebugLine(GetWorld(), ViewPoint + ViewVector * 200, ViewPoint + ViewVector * 1000, FColor::Blue);

	for (auto& item : results)
	{
		FHitResult tempresult;
		FCollisionQueryParams params(NAME_None, true, owner);
		FVector start;
		FVector end;


		FVector impactNormal = item.ImpactNormal;
		impactNormal.Z = 0;


		UKismetSystemLibrary::PrintString(GetWorld(), impactNormal.ToString(), true, false, {0.0f,0.6f ,1.0f ,1.0f}, DeltaTime);
		//UKismetSystemLibrary::PrintString(GetWorld(), item.GetActor()->GetName(), true, false, { 0.0f,0.6f ,1.0f ,1.0f }, DeltaTime);
		DrawDebugSphere(GetWorld(), item.Location, 10, 32, FColor::Blue);
		DrawDebugSphere(GetWorld(), item.ImpactPoint, 10, 32, FColor::Magenta);
		DrawDebugLine(GetWorld(), item.Location, item.Location + item.Normal * 100, FColor::Blue);
		DrawDebugLine(GetWorld(), item.ImpactPoint, item.ImpactPoint + impactNormal * 100, FColor::Magenta);
		DrawDebugLine(GetWorld(), item.ImpactPoint, item.ImpactPoint + impactNormal.RotateAngleAxis(90, FVector::ZAxisVector) * 1000, FColor::Magenta);

		if (item.Normal == item.ImpactNormal) {
			start = item.Location - item.ImpactNormal;
			end = start + (item.Location - item.ImpactPoint).GetSafeNormal() * 1000;
			end.Z -= 1.0;

			GetWorld()->LineTraceSingleByChannel(tempresult, start, end, traceChanel, params);
			DrawDebugLine(GetWorld(), tempresult.Location, tempresult.Location + tempresult.Normal * 100, FColor::Cyan);

		}
		else {
			UKismetSystemLibrary::PrintString(GetWorld(), item.Normal.ToString(), true, false, { 0.0f,0.6f ,1.0f ,1.0f }, DeltaTime);
			UKismetSystemLibrary::PrintString(GetWorld(), item.ImpactNormal.ToString(), true, false, { 0.0f,0.6f ,1.0f ,1.0f }, DeltaTime);

		}

		float ImpactDistance;
		{
			FVector tempvec = (item.Location - item.ImpactPoint);
			tempvec.Z = 0;
			ImpactDistance = tempvec.Size();
		}
		FVector targetVector = item.Location - FMath::Tan(FMath::Acos(item.Normal.GetSafeNormal().Dot((ViewVector.GetSafeNormal2D())))) *
			ImpactDistance * ViewVector + item.Normal * 20;



		
		GetWorld()->LineTraceSingleByChannel(tempresult, targetVector + FVector::UpVector * 500, targetVector + FVector::DownVector * 1000, ECC_Visibility, params);

		start = tempresult.Location + FVector::UpVector * capsule->GetScaledCapsuleHalfHeight();
			
		DrawDebugLine(GetWorld(), start, start - item.ImpactNormal * capsule->GetScaledCapsuleRadius(), FColor::Green);
		GetWorld()->LineTraceSingleByChannel(tempresult, start, start - item.ImpactNormal * capsule->GetScaledCapsuleRadius(), traceChanel, params);

		DrawDebugSphere(GetWorld(), targetVector, 10, 32, FColor::Green);
		if (tempresult.bBlockingHit) {
			DrawDebugCapsule(GetWorld(), tempresult.Location + tempresult.Normal * capsule->GetScaledCapsuleRadius(),
				capsule->GetScaledCapsuleHalfHeight(),
				capsule->GetScaledCapsuleRadius(),
				FQuat::Identity,
				FColor::Green);
		}
	}


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

bool UCoverComponent::isPeeking()
{
	return mPeekingState != EPeekingState::None;
}

void UCoverComponent::CalculateCoverShoot()
{
	mCoverShootingState = ECoverShootingState::None;
	FRotator& aimoffset = m_Weapon->aimOffset;

	FVector forwardVector = owner->GetActorForwardVector() * capsule->GetScaledCapsuleRadius() * 1.1f;
	FVector upVector = owner->GetActorUpVector() * capsule->GetScaledCapsuleHalfHeight() * 2.01f;
	FVector RightVector = owner->GetActorRightVector() * capsule->GetScaledCapsuleRadius() * 1.1f;

	if (!m_Inputdata->IsFire || FMath::Abs(aimoffset.Yaw) > 45.0f) return;
	if (m_FaceRight < 0) {
		aimoffset.Yaw *= -1.0f;
	}
	if (owner->GetControlRotation().Vector().Dot(forwardVector.GetSafeNormal()) < 0) return;
	FVector temppos = owner->GetActorLocation();
	FVector start;
	FVector end;
	FHitResult result;
	FCollisionQueryParams param(NAME_None, true, GetOwner());

	start = temppos + -RightVector;
	end = start + forwardVector;
	result = FHitResult();
	GetWorld()->LineTraceSingleByChannel(result, start, end, traceChanel, param);
	if (!result.GetActor()) {
		DrawDebugLine(GetWorld(), start, end, FColor::Red);
		mCoverShootingState = ECoverShootingState::Left;
		return;
	}

	start = temppos + RightVector;
	end = start + forwardVector;
	result = FHitResult();
	GetWorld()->LineTraceSingleByChannel(result, start, end, traceChanel, param);
	if (!result.GetActor()) {
		DrawDebugLine(GetWorld(), start, end, FColor::Red);
		mCoverShootingState = ECoverShootingState::Right;
		return;
	}

	start = temppos + upVector;
	end = start + forwardVector;
	result = FHitResult();
	GetWorld()->LineTraceSingleByChannel(result, start, end, traceChanel, param);
	if (!result.GetActor()) {
		DrawDebugLine(GetWorld(), start, end, FColor::Red);
		mCoverShootingState = ECoverShootingState::Front;
		return;
	}


	return;
}


ECoverShootingState UCoverComponent::getCoverSootingState()
{
	return mCoverShootingState;
}

EPeekingState UCoverComponent::getPeekingState()
{
	return mPeekingState;
}


void UCoverComponent::StartCover(FHitResult& reslut)
{
	RotateSet(0.0f);
	//m_Movement->SetPlaneConstraintEnabled(true);
	owner->SetActorLocation(reslut.Location + reslut.Normal * capsule->GetScaledCapsuleRadius() * 1.01f);
	m_IsCover = true;
}

void UCoverComponent::StopCover()
{
	//m_Movement->SetPlaneConstraintEnabled(false);
	m_IsCover = false;
	owner->FindComponentByClass<UBaseInputComponent>()->m_CanUnCrouch = true;
}

void UCoverComponent::CheckCoverCollision(OUT FHitResult& result)
{
	FVector start = owner->GetActorLocation() + m_FaceRight * owner->GetActorRightVector() * capsule->GetScaledCapsuleRadius();
	FVector end = start + (owner->GetActorForwardVector() * capsule->GetScaledCapsuleRadius() * 3.0f);
	FCollisionQueryParams params(NAME_None, true, owner);

	GetWorld()->LineTraceSingleByChannel(result, start, end, traceChanel, params);

}

void UCoverComponent::PlayCornering()
{
	FHitResult result1;
	CheckCoverCollision(result1);

	FHitResult result2;
	FVector start = result1.TraceEnd;
	FVector end = start + -m_FaceRight * owner->GetActorRightVector() * capsule->GetScaledCapsuleRadius();
	FCollisionQueryParams params(NAME_None, true, owner);

	GetWorld()->LineTraceSingleByChannel(result2, start, end, traceChanel, params);
	DrawDebugSphere(GetWorld(), end, 10.f, 32, FColor::Cyan, false, 100.0f);
	if (!result2.bBlockingHit) return;

	FVector targetPoint = result2.Location + result2.Normal * capsule->GetScaledCapsuleRadius() * 1.01f;
	DrawDebugSphere(GetWorld(), targetPoint, 10.0f, 32, FColor::Red, false, 100.0f);
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(owner->GetController(), targetPoint);

	for (auto& item : owner->GetComponentsByInterface(UPlayerMovable::StaticClass()))
	{
		Cast<IPlayerMovable>(item)->SetCanMove(false);
	}

	//m_Movement->SetPlaneConstraintEnabled(false);
	m_IsCornering = true;
	m_Turnlookpoint = end;

	owner->GetMovementComponent()->AddInputVector( -m_FaceRight * owner->GetActorRightVector());
}



void UCoverComponent::StopCornering(float DeltaTim)
{
	for (auto& item : owner->GetComponentsByInterface(UPlayerMovable::StaticClass()))
	{
		Cast<IPlayerMovable>(item)->SetCanMove(true);
	}
	
	//m_Movement->SetPlaneConstraintEnabled(true);
	m_IsCornering = false;
	m_IsTurnWait = false;
	m_Turnlookpoint = FVector::ZeroVector;
}

void UCoverComponent::PlayingCornering(float DeltaTim)
{
	owner->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(owner->GetActorLocation(), m_Turnlookpoint));
		
}

void UCoverComponent::BeCrouch(float deltaTime)
{
	FVector forwardVector = owner->GetActorForwardVector() * capsule->GetScaledCapsuleRadius() * 2.01f;
	FVector upVector = owner->GetActorUpVector() * capsule->GetUnscaledCapsuleHalfHeight() * 1.01f;
	owner->bIsCrouched ? upVector *= 2.0f : upVector;
	FVector start;
	FVector end;

	start = owner->GetActorLocation() + upVector;
	if (m_Turnlookpoint == FVector::ZeroVector) {
		end = start + forwardVector;
	}
	else { 
		end = m_Turnlookpoint;
		end.Z = start.Z;
	}
	FHitResult result;
	FCollisionQueryParams param(NAME_None, true, GetOwner());
	GetWorld()->LineTraceSingleByChannel(
		result, start, end, traceChanel, param);
	//DrawDebugLine(GetWorld(), start + upVector, end + upVector, FColor::Magenta, false, 100.0f);
	if (result.bBlockingHit) {
		owner->FindComponentByClass<UBaseInputComponent>()->m_CanUnCrouch = true;
	}
	else {
		owner->FindComponentByClass<UBaseInputComponent>()->m_CanUnCrouch = false;
		if (!owner->bIsCrouched) {
			owner->Crouch();
		}
	}
}

void UCoverComponent::StartPeeking()
{
	if (!m_IsCover) return; 
	if (FMath::Abs(m_Weapon->aimOffset.Yaw) > 80) return;


	FVector forwardVector = owner->GetActorForwardVector() * capsule->GetScaledCapsuleRadius() * 1.1f;
	FVector upVector = owner->GetActorUpVector() * capsule->GetScaledCapsuleHalfHeight() * 2.01f;
	FVector RightVector = owner->GetActorRightVector() * capsule->GetScaledCapsuleRadius() * 1.1f;

	FVector temppos = owner->GetActorLocation();
	FVector start;
	FVector end;
	FHitResult result;
	FCollisionQueryParams param(NAME_None, true, GetOwner());
	mPeekingState = EPeekingState::None;

	if (m_FaceRight > 0) {
		start = temppos;
		end = start + RightVector;
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);
		if (result.GetActor()) return;

		start = end;
		end = start + -upVector * 1.05f;
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);
		if (!result.GetActor()) return;

		start = start;
		end = start + forwardVector;
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);

		if (!result.GetActor()) {
			if (owner->bIsCrouched) {
				mPeekingState |= EPeekingState::LowRightStart;
			}
			else {
				mPeekingState = EPeekingState::HighRightStart;
			}
			return;
		}
		else {
			start = temppos + upVector;
			end = start + forwardVector;
			GetWorld()->LineTraceSingleByChannel(result, start, end, traceChanel, param);
			if (!result.GetActor()) {
				mPeekingState = EPeekingState::FrontRightStart;
				return;
			}
		}
	}
	else {
		start = temppos;
		end = start + -RightVector;
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);
		if (result.GetActor()) return;

		start = end;
		end = start + -upVector * 1.05f;
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);
		if (!result.GetActor()) return;

		start = start;
		end = start + forwardVector;
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);
		if (!result.GetActor()) {
			if (owner->bIsCrouched) {
				mPeekingState = EPeekingState::LowLeftStart;
			}
			else {
				mPeekingState = EPeekingState::HighLeftStart;
			}
			return;
		}
		else {
			start = temppos + upVector;
			end = start + forwardVector;
			GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);
			if (!result.GetActor()) {
				mPeekingState = EPeekingState::FrontLeftStart;
				return;
			}
		}

	}

	mPeekingState = EPeekingState::None;
}

void UCoverComponent::StopPeeking()
{
	if (!m_IsCover) return;
	mPeekingState = EPeekingState::None;
}
