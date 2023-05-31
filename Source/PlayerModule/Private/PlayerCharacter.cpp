// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerMoveComponent.h"
#include "PlayerWeaponComponent.h"

APlayerCharacter::APlayerCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sk_asset(TEXT("SkeletalMesh'/Game/ThirdPersonKit/Mannequin/Mesh/SKM_Mannequin.SKM_Mannequin'"));
	if (sk_asset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(sk_asset.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));

	FollowSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("FollowSpringArm"));
	FollowSpringArm->SetupAttachment(RootComponent);
	FollowSpringArm->bUsePawnControlRotation = true;
	FollowSpringArm->TargetArmLength = 120.f;
	FollowSpringArm->SocketOffset = FVector(0, 60, 80);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(FollowSpringArm, USpringArmComponent::SocketName);

	playerMove = CreateDefaultSubobject<UPlayerMoveComponent>(TEXT("PlayerMoveComp"));
	weapon = CreateDefaultSubobject<UPlayerWeaponComponent>(TEXT("PlayerWeapon"));
	FName WeaponSocket(TEXT("hand_rSocket"));
	weapon->WeaponMesh->SetupAttachment(GetMesh(), WeaponSocket);
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

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	playerMove->bindInput(PlayerInputComponent);
	weapon->bindInput(PlayerInputComponent);
}

