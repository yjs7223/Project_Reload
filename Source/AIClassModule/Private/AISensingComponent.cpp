// Fill out your copyright notice in the Description page of Project Settings.


#include "AISensingComponent.h"
#include "Engine/DataTable.h"
#include "ST_Range.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Vector.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "AICharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "StatComponent.h"
#include "ST_AIShot.h"
#include <Kismet/GameplayStatics.h>
#include "ST_Spawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "AICommander.h"
#include "SubEncounterSpace.h"
#include "AISpawner.h"
#include "HitImapactDataAsset.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI_Controller.h"
#include "AIWeaponDataAsset.h"
#include "Engine/EngineTypes.h"

UAISensingComponent::UAISensingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ������ ���̺� ����
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/AI_Project/DT/DT_Range.DT_Range'"));
	if (DataTable.Succeeded())
	{
		AIRangeData = DataTable.Object;
	}
}


void UAISensingComponent::BeginPlay()
{
	Super::BeginPlay();
}	


// Called every frame
void UAISensingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ShotSenseRange();
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

bool UAISensingComponent::ShotSenseRange()
{
	DrawCircleSector(AimBwd_Radius, AimBwd_Angle, 50);
	DrawCircleSector(AimFwd_Radius, AimFwd_Angle, 50);
	DrawCircleSector(AimSide_Radius, AimSide_Angle, 50);

	if (IsPlayerInsideFanArea(AimBwd_Radius, AimBwd_Angle, GetOwner()->GetActorForwardVector()))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Sense")));
		sensing = true;
		return true;
	}
	else if (IsPlayerInsideFanArea(AimFwd_Radius, AimFwd_Angle, GetOwner()->GetActorForwardVector()))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Sense")));
		sensing = true;
		return true;
	}
	else if (IsPlayerInsideFanArea(AimSide_Radius, AimSide_Angle, GetOwner()->GetActorForwardVector()))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Sense")));
		sensing = true;
		return true;
	}
	else
	{
		sensing = false;
	}


	return false;
}

bool UAISensingComponent::MinRangeCheck()
{
	if (GetOwner()->GetDistanceTo(GetWorld()->GetFirstPlayerController()->GetPawn()) >= curAIRangeData->Target_MinRange)
	{
		return true;
	}

	return false;
}

void UAISensingComponent::DrawSense()
{
	DrawCircleSector(AimBwd_Radius, AimBwd_Angle, 50);
	DrawCircleSector(AimFwd_Radius, AimFwd_Angle, 50);
	DrawCircleSector(AimSide_Radius, AimSide_Angle, 50);
}

void UAISensingComponent::SetDataTable(FName EnemyName)
{
	curAIRangeData = AIRangeData->FindRow<FST_Range>(EnemyName, TEXT(""));

	AimBwd_Radius = curAIRangeData->AimBwd_Radius;
	AimFwd_Radius = curAIRangeData->AimFwd_Radius;
	AimSide_Radius = curAIRangeData->AimSide_Radius;

	AimBwd_Angle = curAIRangeData->AimBwd_Angle;
	AimFwd_Angle = curAIRangeData->AimFwd_Angle;
	AimSide_Angle = curAIRangeData->AimSide_Angle;
}
