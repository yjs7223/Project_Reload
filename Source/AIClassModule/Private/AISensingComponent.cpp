// Fill out your copyright notice in the Description page of Project Settings.


#include "AISensingComponent.h"
#include "Engine/DataTable.h"
#include "ST_Range.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Vector.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Character.h"

UAISensingComponent::UAISensingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 데이터 테이블 삽입
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/Aws/AI_Stat/DT_Range.DT_Range'"));
	if (DataTable.Succeeded())
	{
		AIRangeData = DataTable.Object;
	}

	// 라이플 데이터 가져오기
	curAIRangeData = AIRangeData->FindRow<FST_Range>("Rifle_E", TEXT(""));
}


void UAISensingComponent::BeginPlay()
{
	Super::BeginPlay();

	
}	


// Called every frame
void UAISensingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UAISensingComponent::DrawCircleSector(float Radius, float Angle, int32 NumSegments)
{
	const FVector CharacterLocation = GetOwner()->GetActorLocation();
	const FVector CharacterForward = GetOwner()->GetActorForwardVector();
	const FRotator CharacterRotation = CharacterForward.Rotation();

	const float TwoPi = 2 * PI;
	const float AngleIncrement = -Angle / NumSegments;
	const float SegmentLength = TwoPi * Radius * (AngleIncrement / 360.0f);

	const FVector StartOffset = FVector(Radius, 0.0f, 0.0f);

	for (int32 i = 0; i < NumSegments; ++i)
	{
		const float tempAngle = (Angle * 0.5) + (i * AngleIncrement);
		const FVector Start = CharacterLocation + CharacterRotation.RotateVector(StartOffset.RotateAngleAxis(tempAngle, FVector::UpVector));
		const FVector End = CharacterLocation + CharacterRotation.RotateVector(StartOffset.RotateAngleAxis(tempAngle + AngleIncrement, FVector::UpVector));

		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1.0f, 0, 1.0f);
	}
}


bool UAISensingComponent::IsPlayerInsideFanArea(float LocationRadius, float FanAngle, FVector FanDirection)
{
	FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector centerLocation = GetOwner()->GetActorLocation();
	FVector locationToPlayer = playerLocation - centerLocation;

	float AngleToPlayer = FMath::Acos(FVector::DotProduct(FanDirection, locationToPlayer.GetSafeNormal()));

	if (AngleToPlayer <= FMath::DegreesToRadians(FanAngle) / 2.0f && locationToPlayer.Size2D() <= LocationRadius)
	{
		return true;
	}

	return false;
}

// 아직 미완성
bool UAISensingComponent::ShotSenseRange()
{
	// 후면
	DrawCircleSector(curAIRangeData->AimBwd_Radius, curAIRangeData->AimBwd_Angle, 50);
	// 옆면
	DrawCircleSector(curAIRangeData->AimFwd_Radius, curAIRangeData->AimFwd_Angle, 50);
	// 정면
	DrawCircleSector(curAIRangeData->AimSide_Radius, curAIRangeData->AimSide_Angle, 50);

	// 가장 가까운 후면부터 검사
	if (IsPlayerInsideFanArea(curAIRangeData->AimBwd_Radius, curAIRangeData->AimBwd_Angle, GetOwner()->GetActorForwardVector()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Sense")));
		sensing = true;
	}
	// 다음 옆면 검사
	else if (IsPlayerInsideFanArea(curAIRangeData->AimFwd_Radius, curAIRangeData->AimFwd_Angle, GetOwner()->GetActorForwardVector()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Sense")));
		sensing = true;
	}
	// 마지막 정면 검사
	else if (IsPlayerInsideFanArea(curAIRangeData->AimSide_Radius, curAIRangeData->AimSide_Angle, GetOwner()->GetActorForwardVector()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Sense")));
		sensing = true;
	}
	else
	{
		sensing = false;
	}


	return true;
}

bool UAISensingComponent::MinRangeCheck()
{
	if (GetOwner()->GetDistanceTo(GetWorld()->GetFirstPlayerController()->GetPawn()) >= curAIRangeData->Target_MinRange)
	{
		return true;
	}

	return false;
}
