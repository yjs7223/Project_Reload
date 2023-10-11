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
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Navigation/PathFollowingComponent.h"
#include "BaseCharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "StatComponent.h"
#include "Pakurable.h"
#define LOCTEXT_NAMESPACE "CoverComponent"

// Sets default values for this component's properties
UCoverComponent::UCoverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	m_CorneringWaitTime = 0.5f;
	// ...
}


// Called when the game starts
void UCoverComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = Cast<ACharacter>(GetOwner());
	m_IsCover = false;
	m_CoverWall = nullptr;

	m_Movement = Cast<UBaseCharacterMovementComponent>(owner->GetCharacterMovement());
	m_Input = owner->FindComponentByClass<UBaseInputComponent>();
	m_Inputdata = m_Input->getInput();
	m_Weapon = owner->FindComponentByClass<UWeaponComponent>();
	capsule = owner->GetCapsuleComponent();
	m_PathFollowingComp = owner->GetController()->FindComponentByClass<UPathFollowingComponent>();

	TArray<UActorComponent*> pakurArr = owner->GetComponentsByInterface(UPakurable::StaticClass());
	if (ensure(pakurArr.Num() == 1)) {
		m_PakurComp = pakurArr[0];
	}

	if (m_PathFollowingComp == nullptr)
	{
		ensure(0 && "GameMode의 플레이어컨트롤러를 APlayerCharactorController로 변경하세요");
		m_PathFollowingComp = NewObject<UPathFollowingComponent>(owner->GetController());
		m_PathFollowingComp->RegisterComponentWithWorld(owner->GetController()->GetWorld());
		m_PathFollowingComp->Initialize();
	}

	m_PathFollowingComp->SetPreciseReachThreshold(0.2f, 0.2f);

	SetIsFaceRight(true);

	if (!Cast<AAIController>(owner->Controller)) {
		
		PlayerCharacterTick.AddUObject(this, &UCoverComponent::SendPlayerUIData);
		PlayerCharacterTick.AddUObject(this, &UCoverComponent::CheckCoverPath);
		PlayerCharacterTick.AddUObject(this, &UCoverComponent::SettingCoverPath);
		PlayerCharacterTick.AddUObject(this, &UCoverComponent::SettingCoverPoint);
		m_PathFollowingComp->OnRequestFinished.AddUObject(this, &UCoverComponent::AIMoveCompleted);
	}
	CoverCharacterTick.AddUObject(this, &UCoverComponent::RotateSet);
	CoverCharacterTick.AddUObject(this, &UCoverComponent::AimSetting);
	CoverCharacterTick.AddUObject(this, &UCoverComponent::CornenringCheck);
	CoverCharacterTick.AddUObject(this, &UCoverComponent::BeCrouch);
	UStatComponent* stat = owner->FindComponentByClass<UStatComponent>();
	stat->Knockback.AddLambda([this](FVector a, bool b) {
		StopCover();
		});
}


// Called every frame
void UCoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PlayerCharacterTick.Broadcast(DeltaTime);

	if (!m_IsCover) return;
	CoverCharacterTick.Broadcast(DeltaTime);
}

void UCoverComponent::PlayCover()
{

	if (!m_PakurComp->GetClass() || IPakurable::Execute_IsRolling(m_PakurComp)) return;
	if (m_Movement->MovementMode == MOVE_Falling) return;

	//플레이어가 이동중에 눌렷다면 엄폐를 중지합니다
	if (EPathFollowingStatus::Type::Moving == m_PathFollowingComp->GetStatus()) {
		StopCover();
		return;
	}
	//엄폐 가능지점이 존재하면 엄폐지점으로 뛰어갑니다
	if (m_CanCoverPoint != FVector::ZeroVector) {
		if (m_IsCover) {
			OnCoverPointsSetDelegate.Broadcast(CalculCoverPath());
		}
	
		if (m_IsCover) {
			m_IsNextCover = true;
		}
		//StopCover(); 
		m_IsCover = false;
		m_Movement->SetMovementMode(MOVE_Custom, CMOVE_Runing);
		m_Inputdata->IsAiming = false;
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(owner->GetController(), m_CanCoverPoint);
		owner->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(owner->GetActorLocation(), m_CanCoverPoint));
		return;
	}
	//없다면 엄폐를 시작합니다
	else {
		StartCover();
	}
	if (m_IsCover) {
		StopCover();
		return;
	}
	
}

void UCoverComponent::SettingMoveVector(OUT FVector& vector)
{
	if (!m_IsCover) return;
	if (m_PeekingState != EPeekingState::None) {
		vector = FVector::ZeroVector;
		return;
	}

	if (owner->GetActorForwardVector().Dot(vector.GetSafeNormal2D()) < -0.9) {
		StopCover();
	}
	else {
		SetIsFaceRight(owner->GetActorRightVector().Dot(vector) > 0);
	}
	FHitResult result;
	CheckCoverCollision(result);
	if (result.bBlockingHit) {
		vector = FaceRight() *owner->GetActorRightVector();
	}
	else {
		vector = FVector::ZeroVector;
		m_IsCorneringWait = true;
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
	if (m_CanCoverPointNormal.Equals(FVector::ZeroVector, 0.1)) {
		m_CanCoverPointNormal = result.Normal;
	}

	m_Movement->SetMovementMode(MOVE_Walking);
	m_CoverWall = result.GetActor();
	m_IsCover = true;
	SetIsFaceRight(true);

	PlayMontageStartCover.Broadcast();
	owner->SetActorRotation((-m_CanCoverPointNormal).Rotation());
	RotateSet(0.0f);
	return m_IsCover;
}

void UCoverComponent::CornenringCheck(float DeltaTime)
{
	OnVisibleCorneringWidget.ExecuteIfBound(m_CurrentCorneringWaitTime != 0.0f, IsFaceRight());
	OnSetPercentCorneringWidget.ExecuteIfBound(m_CurrentCorneringWaitTime);
	if (m_IsCorneringWait) {
		m_IsCorneringWait = false;

		m_CurrentCorneringWaitTime += DeltaTime;

	}
	else {
		m_CurrentCorneringWaitTime = 0.0f;
	}
	if (m_CurrentCorneringWaitTime > m_CorneringWaitTime) {
		m_CurrentCorneringWaitTime = 0.0f;
		StartCornering(); 
		return;
	}

	if (m_IsCornering) {

		if (owner->GetVelocity().Length() == 0) {
			StopCornering(DeltaTime);
		}
		else {
			owner->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(owner->GetActorLocation(), m_Turnlookpoint));
		}
	}
}

TArray<FNavPathPoint> UCoverComponent::CalculCoverPath()
{
	if (m_CanCoverPoint == FVector::ZeroVector) return {};



	AController* Controller = owner->GetController();
	FVector GoalLocation = m_CanCoverPoint;
	UNavigationSystemV1* NavSys = Controller ? FNavigationSystem::GetCurrent<UNavigationSystemV1>(Controller->GetWorld()) : nullptr;
	if (NavSys == nullptr || Controller == nullptr || Controller->GetPawn() == nullptr)
	{
		UE_LOG(LogNavigation, Warning, TEXT("UNavigationSystemV1::SimpleMoveToActor called for NavSys:%s Controller:%s controlling Pawn:%s (if any of these is None then there's your problem"),
			*GetNameSafe(NavSys), *GetNameSafe(Controller), Controller ? *GetNameSafe(Controller->GetPawn()) : TEXT("NULL"));
		return {};
	}
	const FVector AgentNavLocation = Controller->GetNavAgentLocation();
	const ANavigationData* NavData = NavSys->GetNavDataForProps(Controller->GetNavAgentPropertiesRef(), AgentNavLocation);
	if (NavData) {
		FPathFindingQuery Query(Controller, *NavData, AgentNavLocation, GoalLocation);
		FPathFindingResult Result = NavSys->FindPathSync(Query);
		return Result.Path->GetPathPoints();
	}

	return {};
}

void UCoverComponent::SettingCoverPath(float DeltaTime)
{
	m_CoverPath = CalculCoverPath();


}

void UCoverComponent::CheckCoverPath(float DeltaTime)
{
	FVector lastPath = m_CoverPath.Num() > 0 ? m_CoverPath.Last() : FVector::ZeroVector;
	FVector tempPoint = m_CanCoverPoint;
	tempPoint.Z -= owner->GetDefaultHalfHeight();

	if (!lastPath.Equals(tempPoint, 1.0)) {
		m_CanCoverPoint = FVector::ZeroVector;
		m_CoverPath.Empty();
	}
}

void UCoverComponent::AimSetting(float DeltaTime)
{
	FRotator& aimOffset = m_Weapon->aimOffset;

	if (!m_IsCover) return;

	if (m_Inputdata->IsAiming) {
		peekingCheck(aimOffset);
		if (m_PeekingState != EPeekingState::None) {

			if (!IsFaceRight()) aimOffset.Yaw *= -1.0f;
			return;
		}
	}
	else {
		m_PeekingState = EPeekingState::None;
	}
	

	if (IsPeeking()) return;
	
	if (aimOffset.Yaw > 0) {
		aimOffset.Yaw -= 180;
		if ((m_Inputdata->IsAiming || m_Inputdata->IsFire)) {
			SetIsFaceRight(true);
		}
	}
	else {
		aimOffset.Yaw += 180;
		aimOffset.Yaw *= -1.0f;
		if (m_Weapon->m_CanShooting && !m_Inputdata->IsReload) {
			SetIsFaceRight(false);
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
	owner->SetActorRotation(FMath::RInterpTo(owner->GetActorRotation(), TargetRotation, DeltaTime, 0.0f));

	//로테이션이 원하는수치에 비슷해지면 포즈세팅
	if (owner->GetActorRotation().Vector().Dot(TargetRotation.Vector()) >= 0.999) {
		m_IsWillPosSetting = true;
		owner->SetActorRotation(TargetRotation);
		
	}


	if (m_IsWillPosSetting) {
		result;
		start = owner->GetActorLocation();
		end = start + (owner->GetActorForwardVector() * owner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 3.0f);

		if (GetWorld()->LineTraceSingleByChannel(result, start, end, traceChanel, params)) {
		FVector targetPos = result.Location + FinalNormal * owner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.01f;
			owner->SetActorLocation(FMath::VInterpTo(owner->GetActorLocation(), targetPos, DeltaTime, 6.0f));
			if ((owner->GetActorLocation() - targetPos).SizeSquared() <= 100) {
				owner->SetActorLocation(targetPos);
				m_IsWillPosSetting = false;
			}
		}
	}

	return;
}

void UCoverComponent::SettingCoverPoint(float DeltaTime)
{
	m_CanCoverPoint = CalculateCoverPoint(DeltaTime);
	//커버가능ui visible 델리게이트 실행

}

FVector UCoverComponent::CalculateCoverPoint(float DeltaTime)
{
	bool PeekingTraceDebug = false;
	EDrawDebugTrace::Type debugtraceType;
	if (PeekingTraceDebug) {
		debugtraceType = EDrawDebugTrace::ForOneFrame;
	}
	else {
		debugtraceType = EDrawDebugTrace::None;
	}
	TArray<FHitResult> results;
	FVector ViewPoint;
	FVector ViewVector;
	FRotator cameraRotation;

	//AI면 실행하지않습니다

	if (!Cast<APlayerController>(owner->Controller)) return FVector::ZeroVector;
	if (EPathFollowingStatus::Type::Moving == m_PathFollowingComp->GetStatus()) return m_CanCoverPoint;
	owner->Controller->GetPlayerViewPoint(ViewPoint, cameraRotation);

	UCameraComponent* camera = owner->FindComponentByClass<UCameraComponent>();
	ViewVector = cameraRotation.Vector();
	
	//박스 트레이스로 엄폐물을 체크 합니다
	if (!UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(),
		ViewPoint + ViewVector * 200,
		ViewPoint + ViewVector * 1500,
		{ 0, camera->OrthoWidth * 0.2f, camera->OrthoWidth },
		cameraRotation,
		{ UEngineTypes::ConvertToObjectType(coverWallType) },
		true,
		{ m_CoverWall },
		debugtraceType,
		results,
		true
	)) {


		return FVector::ZeroVector;
	}
	// 첫 요소를 가져어옵니다
	// <멀티가 하나만 체크하기에 첫요소만 가져옵니다>

	for (auto& iter : results)
	{
		FHitResult item = iter;
		FHitResult result1;
		FHitResult result2;
		FHitResult result3;
		FHitResult result_Result;
		FCollisionQueryParams params(NAME_None, true, owner);
		FVector start;
		FVector end;
		FVector impactNormal;
		FVector pointToLocation = item.Location - item.ImpactPoint;

		if ((owner->GetActorLocation() - item.ImpactPoint).SquaredLength() > 2250000) continue;
		//트레이스의 법선벡터와 히트된 법선벡터가 같다면 법선벡터를 다시계산합니다
		//impactNormal를 세팅해줍니다
		if (item.Normal == item.ImpactNormal) {
			start = item.Location;
			end = item.ImpactPoint - item.ImpactNormal;
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
		float halfHeight = capsule->GetScaledCapsuleHalfHeight() * (owner->bIsCrouched ? 2.0f : 1.0f);

		// impactNormal에서 커서쪽으로 회전한 impactRotatedVector를 만들어줍니다
		isRightRotate = (-pointToLocation).Cross(-impactNormal).Z < 0.0;
		impactRotatedVector = impactNormal.RotateAngleAxis(isRightRotate ? 90 : -90, FVector::ZAxisVector);

		if (PeekingTraceDebug) DrawDebugLine(GetWorld(), item.ImpactPoint, item.ImpactPoint + impactNormal * 500.0f, FColor::Magenta, false, DeltaTime);
		//충돌지점으로부터 살짝 떨어진곳에서 상하 체크를 합니다
		FVector targetVector = FMath::LinePlaneIntersection(ViewPoint, ViewPoint + ViewVector * 1000, item.ImpactPoint, impactNormal) + item.Normal;
		targetVector.Z > item.ImpactPoint.Z ? targetVector.Z : targetVector.Z = item.ImpactPoint.Z;
		FVector temptargetVector;
		start = targetVector;
		end = start + FVector::DownVector * 1000;
		GetWorld()->LineTraceSingleByChannel(result2, start, end, ECC_Visibility, params);
		if (PeekingTraceDebug) DrawDebugLine(GetWorld(), start, end, FColor::Red, false, DeltaTime);

		// 위에선 계산한 살짝 떨어진곳에서 벽쪽으로 라인체크를 합니다
		// <impactRotatedVector * capsule->GetScaledCapsuleRadius() * 2.0f> 로 값을 살짝 보정해줍니다
		start = result2.Location + FVector::UpVector * halfHeight + impactRotatedVector * capsule->GetScaledCapsuleRadius() * 2.5f;
		end = start - impactNormal * capsule->GetScaledCapsuleRadius();
		GetWorld()->LineTraceSingleByChannel(result3, start, end, traceChanel, params);
		if (PeekingTraceDebug) DrawDebugLine(GetWorld(), start, end, FColor::Green, false, DeltaTime);


		if (result3.bBlockingHit) {
			//맞았다면 <impactRotatedVector * capsule->GetScaledCapsuleRadius() * 2.0f>을 제외해서 다시 계산해서 result_Result에 넣어줍니다
			start = result2.Location + FVector::UpVector * halfHeight;
			end = start - impactNormal * capsule->GetScaledCapsuleRadius();
			GetWorld()->LineTraceSingleByChannel(result_Result, start, end, traceChanel, params);
			if (PeekingTraceDebug) DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, DeltaTime);
		}
		else {
			//맞지않았다면 트레이스가 끝난지점에서 커서쪽으로 체크하여
			// result_Result를 벽쪽으로 고정시켜줍니다
			FHitResult result4;

			start = result3.TraceEnd;
			end = start + impactRotatedVector * -1000;
			GetWorld()->LineTraceSingleByChannel(result4, start, end, traceChanel, params);
			if (PeekingTraceDebug) DrawDebugLine(GetWorld(), start, end, FColor::Silver, false, DeltaTime);

			start = result4.Location + -impactRotatedVector * capsule->GetScaledCapsuleRadius() + impactNormal * capsule->GetScaledCapsuleRadius() * 3.0f;
			end = result4.Location + -impactRotatedVector * capsule->GetScaledCapsuleRadius();
			GetWorld()->LineTraceSingleByChannel(result_Result, start, end, traceChanel, params);
			if (PeekingTraceDebug) DrawDebugLine(GetWorld(), start, end, FColor::Emerald, false, DeltaTime);
			if (PeekingTraceDebug) DrawDebugSphere(GetWorld(), result_Result.Location, 10.0f, 32, FColor::Red);
			if (/*result_Result.ImpactNormal.Dot(impactNormal) < 0.99999 ||*/ result_Result.GetActor() != item.GetActor()) continue;
		}

		temptargetVector = result_Result.Location +
			result_Result.ImpactNormal * capsule->GetUnscaledCapsuleRadius() * 1.01f;
		m_CanCoverPointNormal = result_Result.ImpactNormal;

		return temptargetVector;

	}
	//auto iter = results.CreateConstIterator();
	return  FVector::ZeroVector;
}

void UCoverComponent::SetCanCoverPoint(FVector point)
{
	m_CanCoverPoint = point;
}

bool UCoverComponent::IsCover()
{
	return m_IsCover;
}

bool UCoverComponent::IsNextCover()
{
	return m_IsNextCover;
}

bool UCoverComponent::IsTurnWait()
{
	return m_IsCorneringWait;
}

float UCoverComponent::FaceRight()
{
	return m_FaceRight;
}

bool UCoverComponent::IsFaceRight()
{
	return IsCover() ? FaceRight() > 0.0f : true;
}

void UCoverComponent::SetIsFaceRight(bool faceRight)
{
	//기존 FaceRight를 바꿔야 할떄
	if ((FaceRight() > 0.0f) != faceRight) {
		//위젯위치변경델리게이트실행
		OnFaceRightCoverWidget.ExecuteIfBound(faceRight);
		faceRight ? m_FaceRight = 1.0f : m_FaceRight = -1.0f;
		m_Weapon->SetHandleing(faceRight);
	}
}

bool UCoverComponent::IsCornering()
{
	return m_IsCornering;
}

bool UCoverComponent::IsPeeking()
{
	return m_IsPeeking;
}

void UCoverComponent::SetIsPeeking(bool isPeeking)
{
	m_IsPeeking = isPeeking;
}

void UCoverComponent::CalculateCoverShoot(float DeltaTime)
{
	mCoverShootingState = ECoverShootingState::None;
	FRotator& aimoffset = m_Weapon->aimOffset;

	FVector forwardVector = owner->GetActorForwardVector() * capsule->GetScaledCapsuleRadius() * 1.1f;
	FVector upVector = owner->GetActorUpVector() * capsule->GetScaledCapsuleHalfHeight() * 2.01f;
	FVector RightVector = owner->GetActorRightVector() * capsule->GetScaledCapsuleRadius() * 1.1f;

	if (!m_Inputdata->IsFire || FMath::Abs(aimoffset.Yaw) < 45.0f || m_Inputdata->IsAiming) return;
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
		mCoverShootingState = ECoverShootingState::Left;
		return;
	}

	start = temppos + RightVector;
	end = start + forwardVector;
	result = FHitResult();
	GetWorld()->LineTraceSingleByChannel(result, start, end, traceChanel, param);
	if (!result.GetActor()) {
		mCoverShootingState = ECoverShootingState::Right;
		return;
	}

	start = temppos + upVector;
	end = start + forwardVector;
	result = FHitResult();
	GetWorld()->LineTraceSingleByChannel(result, start, end, traceChanel, param);
	if (!result.GetActor()) {
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
	return m_PeekingState;
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
	if (m_CanCoverPointNormal.Equals(FVector::ZeroVector, 0.1)) {
		m_CanCoverPointNormal = result.Normal;
	}

	m_Movement->SetMovementMode(MOVE_Walking);
	m_CoverWall = result.GetActor();
	m_IsCover = true;
	SetIsFaceRight(m_CanCoverPointNormal.Cross(owner->GetActorForwardVector()).Z < 0);

	PlayMontageStartCover.Broadcast();
	return true;
}


void UCoverComponent::StopCover()
{
	if (m_IsCover) {
		PlayMontageEndCover.Broadcast();
	}

	for (auto& item : owner->GetComponentsByInterface(UPlayerMovable::StaticClass()))
	{
		Cast<IPlayerMovable>(item)->SetCanMove(true);
	}

	m_Movement->SetMovementMode(MOVE_Walking);
	m_CoverWall = nullptr;
	m_IsCover = false;
	mCoverShootingState = ECoverShootingState::None;
	m_PeekingState = EPeekingState::None;
	SetIsFaceRight(true);
	m_CanCoverPointNormal = FVector::ZeroVector;

	m_PathFollowingComp->AbortMove(*this, FPathFollowingResultFlags::MovementStop);
	m_Input->m_CanUnCrouch = true;
	m_Weapon->m_CanShooting = false;

	OnVisibleCorneringWidget.ExecuteIfBound(false, true);
}

void UCoverComponent::CheckCoverCollision(OUT FHitResult& result)
{
	FVector start = owner->GetActorLocation() + FaceRight() * owner->GetActorRightVector() * capsule->GetScaledCapsuleRadius();
	FVector end = start + (owner->GetActorForwardVector() * capsule->GetScaledCapsuleRadius() * 2.0f);
	FCollisionQueryParams params(NAME_None, true, owner);

	GetWorld()->LineTraceSingleByChannel(result, start, end, traceChanel, params);

}

void UCoverComponent::StartCornering()
{
	FHitResult result1;
	CheckCoverCollision(result1);

	FHitResult result2;
	FVector start = result1.TraceEnd;
	FVector end = start + - FaceRight() * owner->GetActorRightVector() * capsule->GetScaledCapsuleRadius();
	FCollisionQueryParams params(NAME_None, true, owner);

	GetWorld()->LineTraceSingleByChannel(result2, start, end, traceChanel, params);
	if (!result2.bBlockingHit) return;
	m_CanCoverPointNormal = result2.Normal;
	FVector targetPoint = result2.Location + result2.Normal * capsule->GetScaledCapsuleRadius() * 1.01f;
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
	m_IsCorneringWait = false;
	m_Turnlookpoint = FVector::ZeroVector;
}

void UCoverComponent::PlayingCornering(float DeltaTim)
{
	owner->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(owner->GetActorLocation(), m_Turnlookpoint));
		
}

void UCoverComponent::BeCrouch(float deltaTime)
{
	if (m_PeekingState != EPeekingState::None) return;
	
	if (isMustCrouch()) {
		m_Input->m_CanUnCrouch = false;
		if (!owner->bIsCrouched) {
			owner->Crouch();
			//if (owner->GetCharacterMovement()->bWantsToCrouch);
		}
	}
	else {
		m_Input->m_CanUnCrouch = true;
	}
}

bool UCoverComponent::isMustCrouch()
{
	FVector forwardVector = owner->GetActorForwardVector() * capsule->GetScaledCapsuleRadius() * 3.01f;
	FVector upVector = owner->GetActorUpVector() * capsule->GetUnscaledCapsuleHalfHeight();
	upVector *= owner->bIsCrouched ? 2.0f : 0.5f;
	FVector start = owner->GetActorLocation() + upVector;
	FVector end;

	if (m_Turnlookpoint == FVector::ZeroVector) {
		end = start + forwardVector;
	}
	else {
		end = m_Turnlookpoint;
		end.Z = start.Z;
	}

	FHitResult result;
	FCollisionQueryParams param(NAME_None, true, GetOwner());
	TArray<AActor*> OutActors;

	/*FHitResult tempResult;
	return !UKismetSystemLibrary::BoxTraceSingle(GetWorld(),
		owner->GetActorLocation() + upVector,
		end,
		{ capsule->GetScaledCapsuleRadius(), capsule->GetScaledCapsuleRadius(), 2.0 },
		owner->GetActorRotation(),
		UEngineTypes::ConvertToTraceType(traceChanel),
		false,
		{},
		EDrawDebugTrace::None,
		tempResult,
		true);*/


	GetWorld()->LineTraceSingleByChannel(result, start, end, traceChanel, param);

	return !result.bBlockingHit;
}

void UCoverComponent::SendPlayerUIData(float DeltaTime)
{
	OnVisibleCoverWidget.ExecuteIfBound(m_CanCoverPoint);
	if (!(IsCover() || m_IsNextCover)) {
		OnCoverPointsSetDelegate.Broadcast({});
	}
	else {
		OnCoverPointsSetDelegate.Broadcast(m_CoverPath);
	}
}

void UCoverComponent::StartPeeking()
{
	if (!m_IsCover) return; 
	if (m_PeekingState != EPeekingState::None) return;
	AController* controller = owner->GetController();
	if (!m_Weapon->IsWeaponBlocking() && Cast<APlayerController>(controller)) return;

	FVector forwardVector = owner->GetActorForwardVector() * capsule->GetScaledCapsuleRadius() * 1.1f;
	FVector upVector = owner->GetActorUpVector() * capsule->GetScaledCapsuleHalfHeight() * 2.01f;
	FVector RightVector = owner->GetActorRightVector() * capsule->GetScaledCapsuleRadius() * 1.1f;

	FVector temppos = owner->GetActorLocation();
	FVector start;
	FVector end;
	FHitResult result;
	FCollisionQueryParams param(NAME_None, true, GetOwner());
	m_PeekingState = EPeekingState::None;

	if (IsFaceRight()) {
		start = temppos;
		end = start + RightVector;
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);
		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 15.0f);
		if (result.GetActor()) return;

		start = end;
		end = start + -upVector * 1.05f;
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);
		DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 15.0f);
		if (!result.GetActor()) return;

		start = start;
		end = start + forwardVector * 1.5f;
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);
		DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, 15.0f);

		if (!result.GetActor() && Cast<APlayerController>(controller)) {
			if (owner->bIsCrouched) {
				m_PeekingState |= EPeekingState::LowRight;
			}
			else {
				m_PeekingState = EPeekingState::HighRight;
			}
			return;
		}
		else {
			start = temppos + upVector;
			end = start + forwardVector * 1.5f;
			GetWorld()->LineTraceSingleByChannel(result, start, end, traceChanel, param);
			//DrawDebugLine(GetWorld(), start, end, FColor::Magenta, false, 15.0f);
			if (!result.GetActor()) {
				m_PeekingState = EPeekingState::FrontRight;
				owner->UnCrouch();
				return;
			}
		}
	}
	else {
		start = temppos;
		end = start + -RightVector;
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);
		//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 15.0f);
		if (result.GetActor()) return;

		start = end;
		end = start + -upVector * 1.05f;
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);
		//DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 15.0f);
		if (!result.GetActor()) return;

		start = start;
		end = start + forwardVector * 1.5f;
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);
		//DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, 15.0f);
		if (!result.GetActor() && Cast<APlayerController>(controller)) {
			if (owner->bIsCrouched) {
				m_PeekingState = EPeekingState::LowLeft;
			}
			else {
				m_PeekingState = EPeekingState::HighLeft;
			}
			return;
		}
		else {
			start = temppos + upVector;
			end = start + forwardVector * 1.5f;
			GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param);
			//DrawDebugLine(GetWorld(), start, end, FColor::Magenta, false, 15.0f);
			if (!result.GetActor()) {
				m_PeekingState = EPeekingState::FrontLeft;
				owner->UnCrouch();
				return;
			}
		}

	}

	m_PeekingState = EPeekingState::None;
}

void UCoverComponent::peekingCheck(FRotator& aimOffset)
{
	if (m_PeekingState == EPeekingState::None) StartPeeking();
	if (!IsPeeking()) return;
	switch (m_PeekingState)
	{
	case EPeekingState::None:
		break;
	case EPeekingState::FrontRight:
		break;
	case EPeekingState::FrontLeft:
		break;
	case EPeekingState::HighRight:
		if (owner->bIsCrouched && !m_Weapon->IsWeaponBlocking()) {
			m_PeekingState = EPeekingState::LowRight;
			owner->Crouch();
		}
		break;
	case EPeekingState::HighLeft:
		if (owner->bIsCrouched && !m_Weapon->IsWeaponBlocking()) {
			m_PeekingState = EPeekingState::LowLeft;
			owner->Crouch();
		}
		break;
	case EPeekingState::LowRight:
		if (m_Weapon->IsWeaponBlocking()) {
			m_PeekingState = EPeekingState::HighRight;
			owner->UnCrouch();
		}
		break;
	case EPeekingState::LowLeft:
		if (m_Weapon->IsWeaponBlocking()) {
			m_PeekingState = EPeekingState::HighLeft;
			owner->UnCrouch();
		}
		break;
	case EPeekingState::MAX:
		break;
	default:
		break;
	}


}


void UCoverComponent::StopPeeking()
{
	if (!m_IsCover) return;
	m_PeekingState = EPeekingState::None;
}

AActor* UCoverComponent::GetCoverWall()
{
	return m_CoverWall;
}

void UCoverComponent::AIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	m_IsNextCover = false;
	if (IsCover()) return;
	if (!Result.IsSuccess()) return;
	if (!StartCover()) return;

	owner->SetActorRotation((-m_CanCoverPointNormal).Rotation());
	RotateSet(0.0f);
}
