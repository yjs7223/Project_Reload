// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "AIWeaponComponent.h"
#include "AICharacterMoveComponent.h"
#include "AIPatrolComponent.h"

AAICharacter::AAICharacter()
{
	AIMovement = CreateDefaultSubobject<UAICharacterMoveComponent>(TEXT("AIMovement"));
	AIWeapon = CreateDefaultSubobject<UAIWeaponComponent>(TEXT("AIWeapon"));
	//AIPatrol = CreateDefaultSubobject<UAIPatrolComponent>(TEXT("AIPatrol"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sk_asset(TEXT("SkeletalMesh'/Game/Animation/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (sk_asset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(sk_asset.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	FName WeaponSocket(TEXT("hand_r_Socket"));
	AIWeapon->WeaponMesh->SetupAttachment(GetMesh(), WeaponSocket);

}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
