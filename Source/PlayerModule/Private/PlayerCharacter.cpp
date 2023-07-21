// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerMoveComponent.h"
#include "PlayerWeaponComponent.h"
#include "PlayerStatComponent.h"
#include "PlayerInputComponent.h"
#include "CoverComponent.h"
#include "CameraControllComponent.h"


APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sk_asset(TEXT("SkeletalMesh'/Game/Animation/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (sk_asset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(sk_asset.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	m_FollowSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("FollowSpringArm"));
	m_FollowSpringArm->SetupAttachment(RootComponent);
	m_FollowSpringArm->bUsePawnControlRotation = true;
	m_FollowSpringArm->TargetArmLength = 120.f;
	m_FollowSpringArm->SocketOffset = FVector(0, 60, 80);
	
	m_FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	m_FollowCamera->SetupAttachment(m_FollowSpringArm, USpringArmComponent::SocketName);

	stat = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("PlayerStat"));
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	weapon = CreateDefaultSubobject<UPlayerWeaponComponent>(TEXT("PlayerWeapon"));
	weapon->setWeaponSkeletalMesh(WeaponMesh, TEXT("SkeletalMesh'/Game/ThirdPersonKit/Meshes/WeaponsTPSKitOrginals/Rifle/SKM_Rifle_01.SKM_Rifle_01'"));
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("hand_r_Socket"));

	m_PlayerMove = CreateDefaultSubobject<UPlayerMoveComponent>(TEXT("PlayerMove"));
	m_InputComponent = CreateDefaultSubobject<UPlayerInputComponent>(TEXT("InputComponent"));
	m_CameraControll = CreateDefaultSubobject<UCameraControllComponent>(TEXT("CameraControll"));
	m_CoverComponent = CreateDefaultSubobject<UCoverComponent>(TEXT("CoverComp"));


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

bool APlayerCharacter::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor, const bool* bWasVisible, int32* UserData) const
{

	FName Name_AILineOfSight = FName(TEXT("TestPawnLineOfSight"));
	FHitResult HitResult;
	FVector SightTargetLocation = GetMesh()->GetSocketLocation("neck_01");


	bool hit = GetWorld()->LineTraceSingleByChannel(HitResult, ObserverLocation, SightTargetLocation, ECC_Visibility, FCollisionQueryParams(Name_AILineOfSight, false, IgnoreActor));

	if (!hit || (HitResult.GetActor() && HitResult.GetActor()->IsOwnedBy(this)))
	{
		OutSeenLocation = SightTargetLocation;
		OutSightStrength = 1;
		return true;
	}
	OutSightStrength = 0;
	return false;
}


