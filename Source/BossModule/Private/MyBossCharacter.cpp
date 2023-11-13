// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBossCharacter.h"
//#include "AISpawner.h"
//#include "ST_Spawn.h"
//#include "AICharacter.h"

void AMyBossCharacter::BeginPlay()
{
    Super::BeginPlay();
   // AAISpawner* aa;
    SetActorTickInterval(0.0f);
}

float AMyBossCharacter::GetActorDegree(FVector p_PlayerLocation, FVector p_SelfLocation)
{
    FVector directionToPlayer = p_PlayerLocation - p_SelfLocation;

    float angleInRadians = FMath::Atan2(directionToPlayer.Y, directionToPlayer.X);
    float angleInDegrees = FMath::RadiansToDegrees(angleInRadians);

    return angleInDegrees;
}

void AMyBossCharacter::MoveInCircle(FVector Origin, float Radius, float Angle)
{
    FVector newLocation;
    float AngleInRadians = FMath::DegreesToRadians(Angle);

    newLocation.X = Origin.X + Radius * FMath::Cos(AngleInRadians);
    newLocation.Y = Origin.Y + Radius * FMath::Sin(AngleInRadians);
    newLocation.Z = Origin.Z;
    
    SetActorLocation(newLocation);
}

FVector AMyBossCharacter::GetLocationInCircle(FVector Origin, float Radius, float Angle)
{
    FVector newLocation;
    float AngleInRadians = FMath::DegreesToRadians(Angle);

    newLocation.X = Origin.X + Radius * FMath::Cos(AngleInRadians);
    newLocation.Y = Origin.Y + Radius * FMath::Sin(AngleInRadians);
    newLocation.Z = Origin.Z;

    return newLocation;
}
