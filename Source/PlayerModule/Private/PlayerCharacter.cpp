// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APlayerCharacter::APlayerCharacter()
{
	FollowSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("FollowSpringArm"));
	FollowSpringArm->SetupAttachment(RootComponent);
	FollowSpringArm->bUsePawnControlRotation = true;
	FollowSpringArm->TargetArmLength = 120.f;
	FollowSpringArm->SocketOffset = FVector(0, 60, 80);


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(FollowSpringArm, USpringArmComponent::SocketName);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
