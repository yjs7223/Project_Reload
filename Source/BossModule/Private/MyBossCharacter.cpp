// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBossCharacter.h"

float AMyBossCharacter::GetActorDegree(FVector p_PlayerLocation, FVector p_SelfLocation)
{
    FVector directionToPlayer = p_PlayerLocation - p_SelfLocation;

    float angleInRadians = FMath::Atan2(directionToPlayer.Y, directionToPlayer.X);
    float angleInDegrees = FMath::RadiansToDegrees(angleInRadians);

    return angleInDegrees;
}

FVector AMyBossCharacter::MoveInCircle(FVector p_Center, float p_Radius, float p_Speed, float Angle)
{
    FVector newLocation;
    newLocation.X = p_Center.X + p_Radius * FMath::Cos(Angle);
    newLocation.Y = p_Center.Y + p_Radius * FMath::Sin(Angle);
    newLocation.Z = p_Center.Z;
    
    return newLocation;
}
