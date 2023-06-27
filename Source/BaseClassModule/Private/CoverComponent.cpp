// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
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

}

void UCoverComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{


}


// Called every frame
void UCoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
	if (!isCovering()) return;

	Moving(DeltaTime);
	FixPos(DeltaTime);
	Cornering(DeltaTime);
	FixAim(DeltaTime);
	CalculatePoint(DeltaTime);
	CalculateCoverShoot(DeltaTime);
	BeCrouch(DeltaTime);

}

void UCoverComponent::bindInput(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Cover", IE_Pressed, this, &UCoverComponent::CoverInputEvent);

	PlayerInputComponent->BindAxis("MoveForward", this, &UCoverComponent::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &UCoverComponent::MoveRight);

	PlayerInputComponent->BindAction("Aiming", IE_Pressed, this, &UCoverComponent::StartAiming);
	PlayerInputComponent->BindAction("Aiming", IE_Released, this, &UCoverComponent::StopAiming);
	PlayerInputComponent->BindAction("Aiming", IE_Pressed, this, &UCoverComponent::StartPeeking);
	PlayerInputComponent->BindAction("Aiming", IE_Released, this, &UCoverComponent::StopPeeking);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &UCoverComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &UCoverComponent::StopFire);
}

void UCoverComponent::CoverInputEvent()
{

	if (mCoverWalls.IsEmpty()) return;

	if (mIsCovering) {
		OffCover();
	}
	else {
		OnCover();
	}
}

void UCoverComponent::MoveForward(float Value)
{
	mMoveVec.X = Value;
}

void UCoverComponent::MoveRight(float Value)
{
	mMoveVec.Y = Value;
}

void UCoverComponent::StartAiming()
{
	mIsAiming = true;
}

void UCoverComponent::StopAiming()
{
	mIsAiming = false;
}

void UCoverComponent::StartFire()
{
	mIsFire = true;
}

void UCoverComponent::StopFire()
{
	mIsFire = false;
}


bool UCoverComponent::isCovering()
{
	return mIsCovering;
}

bool UCoverComponent::isFaceRight()
{
	return mIsFaceRight;
}

bool UCoverComponent::isCornering()
{
	return mIsConering;
}

bool UCoverComponent::isAiming()
{
	//if (isCovering() && mAimYaw < -135) return false;

	return mIsAiming;
}


float UCoverComponent::getAimYaw()
{
	return mAimYaw;
}

float UCoverComponent::getAimPitch()
{
	return mAimPitch;
}

EPeekingState UCoverComponent::getPeekingState()
{
	return mPeekingState;
}

ECoverShootingState UCoverComponent::getCoverSootingState()
{
	return mCoverShootingState;
}

bool UCoverComponent::isPeeking()
{
	return mPeekingState != EPeekingState::None;
}

bool UCoverComponent::isFire()
{
	return mIsFire;
}

bool UCoverComponent::isConerWait()
{
	return mIsCornerWait;
}

void UCoverComponent::OnCover()
{
	//int temppoint;
	//for (auto& item : mCoverWalls)
	//{
	//	temppoint = item->getCalculatePoint(Owner);
	//	if (temppoint == -1) continue;
	//	else
	//	{
	//		mCoveringWall = item;
	//		break;
	//	}
	//}
	//if (!mCoveringWall) {
	//	return;
	//}
	//mIsCovering = true;
	//mCurrPoint = temppoint;

	//mTargetPos = Owner->GetActorLocation();

	//if (mPlayerMove) {
	//	mPlayerMove->SetComponentTickEnabled(false);
	//}
}

void UCoverComponent::OffCover()
{
	//if (mIsConering) return;
	//mCoveringWall = nullptr;
	//mIsCovering = false;
	//mCurrPoint = 0;
	//mIsConering = false;
	//mIsFaceRight = true;
	//mIsConerTime = -1.0f;
	//mIsConeringRight = -1;
	//if (mPlayerMove) {
	//	mPlayerMove->SetComponentTickEnabled(true);
	//	mPlayerMove->SetCanUnCrouch(true);
	//}
	//else {
	//	//ai
	//	Owner->UnCrouch();
	//}
}

void UCoverComponent::StartPeeking()
{
	if (!isCovering()) return;
	if (FMath::Abs(mAimYaw) > 90) return;
	FVector temppos = Owner->GetActorLocation();
	FVector start;
	FVector end;
	FHitResult result;
	FCollisionQueryParams param(NAME_None, true, GetOwner());
	mPeekingState = EPeekingState::None;

	if (mIsFaceRight) {
		start = temppos;
		end = start + mRightVector;
		result = FHitResult();
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);
		DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, INFINITY);
		if (result.GetActor()) return;

		start = end;
		end = start + mDownVector * 1.05f;
		result = FHitResult();
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);
		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, INFINITY);
		if (!result.GetActor()) return;

		start = start;
		end = start + mFrontVector;
		result = FHitResult();
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_GameTraceChannel5, param);
		DrawDebugLine(GetWorld(), start, end, FColor::Green, false, INFINITY);

		if (!result.GetActor()) {
			if (Owner->bIsCrouched) {
				mPeekingState |= EPeekingState::LowRightStart;
			}
			else {
				mPeekingState = EPeekingState::HighRightStart;
			}
			return;
		}
		else {
			start = temppos + mUpVector;
			end = start + mFrontVector;
			result = FHitResult();
			GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_GameTraceChannel5, param);
			if (!result.GetActor()) {
				mPeekingState = EPeekingState::FrontRightStart;
				return;
			}
		}
	}
	else {
		start = temppos;
		end = start + mLeftVector;
		result = FHitResult();
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);
		DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, INFINITY);
		if (result.GetActor()) return;

		start = end;
		end = start + mDownVector * 1.05f;
		result = FHitResult();
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);
		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, INFINITY);
		if (!result.GetActor()) return;

		start = start;
		end = start + mFrontVector;
		result = FHitResult();
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_GameTraceChannel5, param);
		DrawDebugLine(GetWorld(), start, end, FColor::Green, false, INFINITY);
		if (!result.GetActor()) {
			if (Owner->bIsCrouched) {
				mPeekingState = EPeekingState::LowLeftStart;
			}
			else {
				mPeekingState = EPeekingState::HighLeftStart;
			}
			return;
		}
		else {
			start = temppos + mUpVector;
			end = start + mFrontVector;
			result = FHitResult();
			GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_GameTraceChannel5, param);
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
	mPeekingState = EPeekingState::None;
}


FVector UCoverComponent::projectionToWay(const FVector& vec)
{
	//if (mCoveringWall) {
	//	FVector tempvec = vec;
	//	tempvec.Z = 0;
	//	return tempvec.ProjectOnTo(mCoveringWall->getVector(mCurrPoint));
	//}
	//else {
	//	return FVector::ZeroVector;
	//}
	return FVector::ZeroVector;
}

void UCoverComponent::Moving(float deltaTime)
{
	//if (mIsConering || isPeeking()) return;
	//FVector nomalizedvec = mMoveVec.GetSafeNormal();
	//if (nomalizedvec == FVector::ZeroVector) return;

	//FVector tempvec = Owner->Controller->GetControlRotation().RotateVector(nomalizedvec);
	//Owner->GetMovementComponent()->AddInputVector(projectionToWay(tempvec) * moveSpeed);

	//mCoveringWall->getVector(mCurrPoint).Dot(tempvec) <= 0 ? mIsFaceRight = true : mIsFaceRight = false;
}

void UCoverComponent::Cornering(float deltaTime)
{/*
	if (mIsConerTime != -1.0f) {
		mIsConerTime -= deltaTime;
		if (mIsConerTime < 0.0f) {
			mIsConering = true;
			mIsConerTime = 0.0f;
		}
	}

	if (mIsConering) {
		mIsCornerWait = false;
		mConeringAngle += mIsConeringRight == 1 ? 100.f * deltaTime : -100.f * deltaTime;
		if (mConeringAngle < -90.0f || mConeringAngle > 90.0f) {

			mCurrPoint = mCoveringWall->getCalculatePoint(Owner);
			mIsConering = false;
			mIsConerTime = -1.0f;
			mIsConeringRight = -1;
		}
	}

	if (!mIsConering) {
		mConeringAngle = 0.0f;
	}*/
}

void UCoverComponent::FixAim(float deltaTime)
{
	/*
	mAimYaw = mWeapon->mAimYaw;
	mAimPitch = mWeapon->mAimPitch;

	if (isPeeking()) {
		if (!mIsFaceRight) mAimYaw *= -1.0f;
		mWeapon->mAimYaw = mAimYaw;
		return;
	}

	if (mAimYaw > 0) {
		if (isAiming() || (isFire() && mAimYaw > 45)) {
			mAimYaw -= 180;
			mIsFaceRight = true;
		}
	}
	else {
		if (isAiming() || (isFire() && mAimYaw < -45)) {
			mAimYaw += 180;
			mAimYaw *= -1.0f;
			mIsFaceRight = false;
		}
	}
	mWeapon->mAimYaw = mAimYaw;
	mWeapon->mAimPitch = mAimPitch;*/

}

void UCoverComponent::BeCrouch(float deltaTime)
{/*
	FVector start = Owner->GetActorLocation() + mUpVector;
	FVector end = start + mFrontVector;
	FHitResult result;
	FCollisionQueryParams param(NAME_None, true, GetOwner());
	GetWorld()->LineTraceSingleByChannel(
		result, start, end, ECC_GameTraceChannel5, param);
	DrawDebugLine(GetWorld(), start, end, FColor::Magenta);
	if (result.GetActor()) {
		if (mPlayerMove) {
			mPlayerMove->SetCanUnCrouch(true);
		}
	}
	else {
		if (mPlayerMove) {
			mPlayerMove->SetCanUnCrouch(false);
		}
		if (!Owner->bIsCrouched) {
			Owner->Crouch();
		}
	}*/
}

void UCoverComponent::CalculatePoint(float deltaTime)
{
	//FRotator temprot = Owner->GetActorRotation();
	//mUpVector = temprot.RotateVector(FVector(0, 0, mHidingCollider->GetUnscaledCapsuleHalfHeight()));
	//mDownVector = temprot.RotateVector(FVector(0, 0, -mHidingCollider->GetScaledCapsuleHalfHeight()));
	//mFrontVector = temprot.RotateVector(FVector(mHidingCollider->GetScaledCapsuleHalfHeight(), 0, 0));
	//mRightVector = temprot.RotateVector(FVector(0, mHidingCollider->GetUnscaledCapsuleRadius(), 0));
	//mLeftVector = temprot.RotateVector(FVector(0, -mHidingCollider->GetUnscaledCapsuleRadius(), 0));
}


void UCoverComponent::CalculateCoverShoot(float deltaTime)
{
	mCoverShootingState = ECoverShootingState::None;

	if (!isFire() || FMath::Abs(mAimYaw) > 45.0f) return;
	if (!isFaceRight()) {
		mAimYaw *= -1.0f;
	}
	if (Owner->GetControlRotation().Vector().Dot(mFrontVector.GetSafeNormal()) < 0) return;
	FVector temppos = Owner->GetActorLocation();
	FVector start;
	FVector end;
	FHitResult result;
	FCollisionQueryParams param(NAME_None, true, GetOwner());

	//start = temppos + mLeftVector;
	//end = start + mFrontVector;
	//result = FHitResult();
	//GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_EngineTraceChannel2, param);
	//if (!result.GetActor()) {
	//	mCoverShootingState = ECoverShootingState::Left;
	//	return;
	//}

	//start = temppos + mRightVector;
	//end = start + mFrontVector;
	//result = FHitResult();
	//GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_EngineTraceChannel2, param);
	//if (!result.GetActor()) {
	//	mCoverShootingState = ECoverShootingState::Right;
	//	return;
	//}

	start = temppos + mUpVector;
	end = start + mFrontVector;
	result = FHitResult();
	GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_GameTraceChannel5, param);
	if (!result.GetActor()) {
		mCoverShootingState = ECoverShootingState::Front;
		return;
	}


	return;

}

//ACoverWall* UCoverComponent::GetCoverWall()
//{
//	return mCoverWall;
//}

void UCoverComponent::FixPos(float deltaTime)
{
	//float capsuleRadius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
	//FVector2D wayVector = FVector2D(mCoveringWall->getVector(mCurrPoint));
	//FVector2D playerPos = FVector2D(Owner->GetActorLocation() - mCoveringWall->getPoint(mCurrPoint));
	//FVector2D wayVectorN = wayVector.GetSafeNormal();
	//double wayDotPlayer = wayVectorN.Dot(playerPos);
	//FVector2D projectedLocation = wayVectorN * wayDotPlayer;
	//if (mIsConering) {
	//	if (wayDotPlayer * wayDotPlayer > wayVector.SizeSquared()) {
	//		projectedLocation = wayVector;
	//	}
	//	else if (wayDotPlayer < 0) {
	//		projectedLocation = FVector2D::ZeroVector;
	//	}
	//}
	//else {


	//	FVector2D tempvec = wayVector - wayVectorN * capsuleRadius;
	//	if (wayDotPlayer * wayDotPlayer > tempvec.SizeSquared()) {
	//		projectedLocation = tempvec;
	//		if (mIsConerTime == -1.0f) {
	//			if ((mCoveringWall->getVector(mCoveringWall->WrapIndex(mCurrPoint - 1)) * 0.5f).SizeSquared() > capsuleRadius * capsuleRadius) {
	//				mIsConeringRight = 0;
	//				mIsConerTime = turnLimitTime;
	//			}
	//			mIsCornerWait = true;
	//		}
	//	}
	//	else if (wayDotPlayer * wayDotPlayer < capsuleRadius * capsuleRadius) {
	//		projectedLocation = wayVectorN * capsuleRadius;
	//		if (mIsConerTime == -1.0f) {
	//			if ((mCoveringWall->getVector(mCoveringWall->WrapIndex(mCurrPoint + 1)) * 0.5f).SizeSquared() > capsuleRadius * capsuleRadius) {

	//				mIsConeringRight = 1;
	//				mIsConerTime = turnLimitTime;
	//			}
	//			mIsCornerWait = true;
	//		}
	//	}
	//	else {
	//		mIsConerTime = -1.0f;
	//		mIsCornerWait = false;
	//	}
	//}


	//FVector newpos = FVector(projectedLocation + FVector2D(mCoveringWall->getPoint(mCurrPoint))
	//	+ wayVectorN.GetRotated(-90.0f - mConeringAngle) * capsuleRadius * 1.001f, Owner->GetActorLocation().Z);

	//mTargetPos = FMath::VInterpTo(mTargetPos, newpos, deltaTime, moveSpeed * moveSpeed2);

	//Owner->SetActorLocation(mTargetPos);
	//FVector temp = FVector(wayVectorN.GetRotated(90.0f - mConeringAngle), 0);

	//Owner->SetActorRotation(temp.Rotation());


}
