// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "BaseInputComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include "PlayerMovable.h"
#include "WeaponComponent.h"
#include "EPeekingState.h"
#include "Camera/CameraComponent.h"
#include "MathUtil.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/GameplayStatics.h"


#define LOCTEXT_NAMESPACE "CoverComponent"

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
	m_CoverWall = nullptr;

	m_Movement = owner->GetCharacterMovement();
	m_Inputdata = owner->FindComponentByClass<UBaseInputComponent>()->getInput();
	m_Weapon = owner->FindComponentByClass<UWeaponComponent>();
	capsule = owner->GetCapsuleComponent();

	m_PathFollowingComp = owner->GetController()->FindComponentByClass<UPathFollowingComponent>();
	if (m_PathFollowingComp == nullptr)
	{
		m_PathFollowingComp = NewObject<UPathFollowingComponent>(owner->GetController());
		m_PathFollowingComp->RegisterComponentWithWorld(owner->GetController()->GetWorld());
		m_PathFollowingComp->Initialize();

		m_PathFollowingComp->OnRequestFinished.AddUObject(this, &UCoverComponent::AIMoveCompleted);
		m_PathFollowingComp->SetPreciseReachThreshold(0.2f, 0.2f);
	}


}

// Called every frame
void UCoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	m_CanCoverPoint = CalculateCoverPoint(DeltaTime);
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
	if (EPathFollowingStatus::Type::Moving == m_PathFollowingComp->GetStatus()) {
		m_PathFollowingComp->AbortMove(*this, FPathFollowingResultFlags::MovementStop);
		StopCover();
		return;
	}

	if (m_CanCoverPoint != FVector::ZeroVector) {
		m_Inputdata->IsRuning = true;
		m_IsCover = false;
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(owner->GetController(), m_CanCoverPoint);
		owner->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(owner->GetActorLocation(), m_CanCoverPoint));
		return;
	}
	else {
		StartCover();
	}

	if (m_IsCover) {
		StopCover();
		return;
	}

}

void UCoverComponent::SettingMoveVector(FVector& vector)
{
	if (!m_IsCover) return;
	if (isPeeking()) {
		vector = FVector::ZeroVector;
		return;
	}

	if (owner->GetActorForwardVector().Dot(vector.GetSafeNormal2D()) < -0.9) {
		StopCover();
	}
	SetIsFaceRight(owner->GetActorRightVector().Dot(vector) > 0);
	FHitResult result;
	CheckCoverCollision(result);
	if (result.bBlockingHit) {
		vector = FaceRight() *owner->GetActorRightVector();
	}
	else {
		vector = FVector::ZeroVector;
		m_IsTurnWait = true;
	}
}

bool UCoverComponent::StartAICover()
{
	if (!owner)
	{
		owner = Cast<ACharacter>(GetOwner());
	}
	if (!capsule)
	{
		capsule = owner->GetCapsuleComponent();
	}
	FHitResult result = FHitResult();
	FHitResult temp = FHitResult();

	TArray<AActor*> OutActors;
	if (UKismetSystemLibrary::CapsuleOverlapActors(GetWorld(),
		owner->GetActorLocation(),
		capsule->GetScaledCapsuleRadius() * 2.0f,
		capsule->GetScaledCapsuleHalfHeight() * 2.0f,
		{ UEngineTypes::ConvertToObjectType(coverWallType) },
		AActor::StaticClass(),
		{},
		OutActors))   
	{
		for (auto& item : OutActors)
		{

			FVector start = owner->GetActorLocation();
			FVector end = item->GetActorLocation();
			FCollisionQueryParams params(NAME_None, true, owner);

			if (GetWorld()->LineTraceSingleByChannel(result, start, end, traceChanel, params)) {
				break;
			}
		}

	}

	if (result.GetActor() == nullptr) return false;
	RotateSet(0.0f);

	owner->SetActorLocation(result.Location + result.Normal * capsule->GetScaledCapsuleRadius() * 1.01f);
	m_CoverWall = result.GetActor();
	m_IsCover = true;

	return m_IsCover;
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
		if (!IsFaceRight()) aimOffset.Yaw *= -1.0f;
		return;
	}
	if (m_Inputdata->IsAiming || (m_Inputdata->IsFire && aimOffset.Yaw > 45)) {
		aimOffset.Yaw -= 180;
		SetIsFaceRight(true);

	}
	else if (m_Inputdata->IsAiming || (m_Inputdata->IsFire && aimOffset.Yaw < -45)) {
		aimOffset.Yaw += 180;
		aimOffset.Yaw *= -1.0f;
		SetIsFaceRight(false);
	}
	/*if (aimOffset.Yaw > 0) {
		if (m_Inputdata->IsAiming || (m_Inputdata->IsFire && aimOffset.Yaw > 45)) {
			aimOffset.Yaw -= 180;
			SetIsFaceRight(true);

		}
	}
	else {
		if (m_Inputdata->IsAiming || (m_Inputdata->IsFire && aimOffset.Yaw < -45)) {
			aimOffset.Yaw += 180;
			aimOffset.Yaw *= -1.0f;
			SetIsFaceRight(false);
		}
	}*/
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
	
	//AI로 움직이고있다면
	if (m_PathFollowingComp->GetStatus() == EPathFollowingStatus::Moving) {


		return;
	}

	//앞에 벽이 있는지
	start = owner->GetActorLocation();
	end = start + (owner->GetActorForwardVector() * capsule->GetScaledCapsuleRadius() * 2.0f);
	GetWorld()->LineTraceSingleByChannel(result, start, end, traceChanel, params);

	if (!result.bBlockingHit) return;

	//벽이있다면 이동방향 체크
	start = owner->GetActorLocation() + FaceRight() * owner->GetActorRightVector() * capsule->GetScaledCapsuleRadius();
	end = start + (owner->GetActorForwardVector() * capsule->GetScaledCapsuleRadius() * 2.0f);
	GetWorld()->LineTraceSingleByChannel(result2, start, end, traceChanel, params);

	//이동방향에 벽이있다면 캡슐로 노말값 가져오기
	if (result2.bBlockingHit) {
		FHitResult tempResult;
		start = owner->GetActorLocation();
		end = start + (owner->GetActorForwardVector() * capsule->GetScaledCapsuleRadius() * 2.0f);

		UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), start, end,
			capsule->GetScaledCapsuleRadius(),
			capsule->GetScaledCapsuleHalfHeight(),
			UEngineTypes::ConvertToTraceType(traceChanel),
			false,
			{},
			EDrawDebugTrace::None,
			tempResult,
			true);

		FinalNormal = tempResult.Normal;
	}
	//없다면 액터위치의 노말값가져오기
	else {
		FinalNormal = result.Normal;
	}
	// result.Normal이나 tempResult.Normal이 없다면 에러
	if (FinalNormal == FVector::ZeroVector) {
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ERROR FinalNormal is Zero"), true,true, FColor::Red, 1000.0f);
	}
	//로테이션 가져와서 보간설정
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, -FinalNormal);
	owner->SetActorRotation(FMath::RInterpTo(owner->GetActorRotation(), TargetRotation, DeltaTime, 11.0f));

	//로테이션이 원하는수치에 비슷해지면 포즈세팅
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

FVector UCoverComponent::CalculateCoverPoint(float DeltaTime)
{
	TArray<FHitResult> results;
	FVector ViewPoint;
	FVector ViewVector;
	FRotator cameraRotation;

	if (owner->ActorHasTag("Enemy"))
	{
		return FVector::ZeroVector;
	}

	owner->Controller->GetPlayerViewPoint(ViewPoint, cameraRotation);

	UCameraComponent* camera = owner->FindComponentByClass<UCameraComponent>();
	ViewVector = cameraRotation.Vector();
	if (!UKismetSystemLibrary::BoxTraceMulti(GetWorld(),
		ViewPoint + ViewVector * 200,
		ViewPoint + ViewVector * 1000,
		{ 0, camera->OrthoWidth * 0.2f, camera->OrthoWidth * 0.27f },
		cameraRotation,
		UEngineTypes::ConvertToTraceType(traceChanel),
		true,

		{m_CoverWall},
		EDrawDebugTrace::None,
		results,
		true
	)) return FVector::ZeroVector;

	auto iter = results.CreateConstIterator();
	FHitResult item = *iter;
	FHitResult result1;
	FHitResult result2;
	FHitResult result3;
	FCollisionQueryParams params(NAME_None, true, owner);
	FVector start;
	FVector end;
	FVector impactNormal;
	FVector pointToLocation = item.Location - item.ImpactPoint;

	if (item.Normal == item.ImpactNormal) {
		start = item.Location;
		end = item.ImpactPoint - item.ImpactNormal + FVector::DownVector;
		start.Z = end.Z;
		GetWorld()->LineTraceSingleByChannel(result1, start, end, traceChanel, params);
		impactNormal = result1.Normal;

	}
	else {
		impactNormal = item.ImpactNormal;
	}
	impactNormal.Z = 0;

	FVector impactRotatedVector;
	bool isRightRotate;

	isRightRotate = (-pointToLocation).Cross(-impactNormal).Z < 0.0;
	impactRotatedVector = impactNormal.RotateAngleAxis(isRightRotate ? 90 : -90, FVector::ZAxisVector);

	FVector targetVector = FMath::LinePlaneIntersection(ViewPoint, ViewPoint + ViewVector * 1000, item.ImpactPoint, impactNormal) + item.Normal;
	FVector temptargetVector;
	start = targetVector + FVector::UpVector * 500;
	end = targetVector + FVector::DownVector * 1000;
	GetWorld()->LineTraceSingleByChannel(result2, start, end, ECC_Visibility, params);

	start = result2.Location + FVector::UpVector * capsule->GetScaledCapsuleHalfHeight() + impactRotatedVector * capsule->GetScaledCapsuleRadius() * 2.0f;
	end = start - impactNormal * capsule->GetScaledCapsuleRadius();
	GetWorld()->LineTraceSingleByChannel(result3, start, end, traceChanel, params);

	if (result3.bBlockingHit) {
		FHitResult result4;
		start = result2.Location + FVector::UpVector * capsule->GetScaledCapsuleHalfHeight();
		end = start - impactNormal * capsule->GetScaledCapsuleRadius();
		GetWorld()->LineTraceSingleByChannel(result3, start, end, traceChanel, params);
		targetVector = result3.Location + result3.Normal * 1.1;
		temptargetVector = result3.Location + result3.Normal * capsule->GetScaledCapsuleRadius();
		m_CanCoverPointNormal = result3.Normal;
	}
	else {
		FHitResult result4;
		FHitResult result5;
		start = result3.TraceEnd;
		end = start + impactRotatedVector * -1000;
		GetWorld()->LineTraceSingleByChannel(result4, start, end, traceChanel, params);

		start = result4.Location + -impactRotatedVector * capsule->GetScaledCapsuleRadius() + impactNormal * capsule->GetScaledCapsuleRadius() * 2.5f;
		end = result4.Location + -impactRotatedVector * capsule->GetScaledCapsuleRadius();
		GetWorld()->LineTraceSingleByChannel(result5, start, end, traceChanel, params);
		if (result5.ImpactNormal != impactNormal || result5.GetActor() != item.GetActor()) return FVector::ZeroVector;

		targetVector = result5.Location + result5.ImpactNormal * 1.1;
		temptargetVector = result5.Location + result5.ImpactNormal * capsule->GetScaledCapsuleRadius();
		m_CanCoverPointNormal = result5.ImpactNormal;
	}

	DrawDebugPoint(GetWorld(), targetVector, 5, FColor::Green);
	DrawDebugCapsule(GetWorld(), temptargetVector,
		capsule->GetScaledCapsuleHalfHeight(),
		capsule->GetScaledCapsuleRadius(),
		FQuat::Identity,
		FColor::Green);

	return temptargetVector + FVector::UpVector * capsule->GetScaledCapsuleHalfHeight() * 1.01f;

}

void UCoverComponent::SetCanCoverPoint(FVector point)
{
	m_CanCoverPoint = point;
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
	return m_IsCover ? m_FaceRight : true;
}

bool UCoverComponent::IsFaceRight()
{
	return FaceRight() > 0.0f;
}

void UCoverComponent::SetIsFaceRight(bool faceRight)
{
	if (IsFaceRight() != faceRight) {
		faceRight ? m_FaceRight = 1.0f : m_FaceRight = -1.0f;

		UWeaponComponent* mWeapon = owner->FindComponentByClass<UWeaponComponent>();
		USkeletalMeshComponent* WeaponMesh;
		if (mWeapon) {
			WeaponMesh = mWeapon->WeaponMesh;
		}
		else {
			return;
		}
		FName handSocketName;
		FRotator meshRotate;
		FVector meshLocation;

		if (m_FaceRight >= 0.0f) {
			handSocketName = TEXT("hand_r_Socket");
			meshRotate = FRotator(0.0, 0.0, 0.0);
			meshLocation = WeaponMesh->GetRelativeLocation() * FVector(-1.0f, 1.0f, -1.0f);
		}
		else {
			handSocketName = TEXT("hand_l_Socket");
			meshRotate = FRotator(0.0, 180.0, 180.0);
			meshLocation = WeaponMesh->GetRelativeLocation() * FVector(-1.0f, 1.0f, -1.0f);
		}


		WeaponMesh->AttachToComponent(owner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, handSocketName);
		WeaponMesh->SetRelativeRotation(meshRotate);
		WeaponMesh->SetRelativeLocation(meshLocation);
	}
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
	if (!IsFaceRight()) {
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

FVector UCoverComponent::getCanCoverPoint()
{
	return m_CanCoverPoint;
}

FVector UCoverComponent::GetPointNormal()
{
	return m_CanCoverPointNormal;
}

bool UCoverComponent::StartCover()
{
	FHitResult result = FHitResult();
	FHitResult temp = FHitResult();
	TArray<AActor*> OutActors;
	if (UKismetSystemLibrary::CapsuleOverlapActors(GetWorld(),
		owner->GetActorLocation(),
		capsule->GetScaledCapsuleRadius() * 2.0f,
		capsule->GetScaledCapsuleHalfHeight() * 2.0f,
		{ UEngineTypes::ConvertToObjectType(coverWallType) },
		AActor::StaticClass(),
		{},
		OutActors))
	{
		for (auto& item : OutActors)
		{
			
			FVector start = owner->GetActorLocation();
			FVector end = item->GetActorLocation();
			FCollisionQueryParams params(NAME_None, true, owner);

			if (GetWorld()->LineTraceSingleByChannel(result, start, end, traceChanel, params)) {
				break;
			}
		}

	}
	
	if (result.GetActor() == nullptr) return false;
	RotateSet(0.0f);

	//owner->SetActorLocation(result.Location + result.Normal * capsule->GetScaledCapsuleRadius() * 1.01f);
	m_CoverWall = result.GetActor();
	m_IsCover = true;
	SetIsFaceRight(true);

	return true;
}


void UCoverComponent::StopCover()
{
	m_Inputdata->IsRuning = false;
	m_CoverWall = nullptr;
	m_IsCover = false;
	SetIsFaceRight(true);

	owner->FindComponentByClass<UBaseInputComponent>()->m_CanUnCrouch = true;
}

void UCoverComponent::CheckCoverCollision(OUT FHitResult& result)
{
	FVector start = owner->GetActorLocation() + FaceRight() * owner->GetActorRightVector() * capsule->GetScaledCapsuleRadius();
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
	FVector end = start + - FaceRight() * owner->GetActorRightVector() * capsule->GetScaledCapsuleRadius();
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

	owner->GetMovementComponent()->AddInputVector( - FaceRight() * owner->GetActorRightVector());
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

	if (IsFaceRight()) {
		start = temppos;
		end = start + RightVector;
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);
		if (result.GetActor()) return;

		start = end;
		end = start + -upVector * 1.05f;
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);
		if (!result.GetActor()) return;

		start = start;
		end = start + forwardVector * 1.1f;
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
			end = start + forwardVector * 1.1f;
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
		end = start + forwardVector * 1.1f;
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
			end = start + forwardVector * 1.1f;
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

AActor* UCoverComponent::GetCoverWall()
{
	return m_CoverWall;
}

void UCoverComponent::AIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!Result.IsSuccess()) return;


	StartCover();
	FRotator rot = UKismetMathLibrary::FindLookAtRotation(owner->GetActorLocation(), m_CoverWall->GetActorLocation());
	rot.Pitch = 0.0;
	rot.Roll = 0.0;
	owner->SetActorRotation(rot);

	RotateSet(0.0f);
}
