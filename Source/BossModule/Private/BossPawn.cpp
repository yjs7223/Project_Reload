// Fill out your copyright notice in the Description page of Project Settings.


#include "BossPawn.h"

// Sets default values
ABossPawn::ABossPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

//void ABossPawn::MoveInCircle(FVector p_Center, float p_Radius, float p_Speed, float Angle)
//{
//    FVector newLocation;
//    newLocation.X = p_Center.X + p_Radius * FMath::Cos(Angle);
//    newLocation.Y = p_Center.Y + p_Radius * FMath::Sin(Angle);
//    newLocation.Z = p_Center.Z; 
//    SetActorLocation(newLocation);
//}
//
//float ABossPawn::GetActorDegree(FVector p_PlayerLocation, FVector p_SelfLocation)
//{
//    FVector directionToPlayer = p_PlayerLocation - p_SelfLocation;
//
//    float angleInRadians = FMath::Atan2(directionToPlayer.Y, directionToPlayer.X);
//    float angleInDegrees = FMath::RadiansToDegrees(angleInRadians);
//
//    return angleInDegrees;
//}

// Called when the game starts or when spawned
void ABossPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

