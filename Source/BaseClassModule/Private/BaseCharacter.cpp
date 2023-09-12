// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "StatComponent.h"
#include "WeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "CoverComponent.h"
#include "BaseCharacterMovementComponent.h"


// Sets default values
//ABaseCharacter::ABaseCharacter()
//{
// 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	
//
//	PrimaryActorTick.bCanEverTick = true;
//
//	GetCapsuleComponent()->SetCapsuleRadius(44.0f);
//
//
//	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh> sk_asset(TEXT("SkeletalMesh'/Game/Cyber_Girl/Mesh/SK_Cyber_Girl.SK_Cyber_Girl'"));
//
//	if (sk_asset.Succeeded())
//	{
//		GetMesh()->SetSkeletalMesh(sk_asset.Object);
//	}
//	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));*/
//	//GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
//
//	//GetCapsuleComponent()->SetCapsuleHalfHeight(95);
//
//
//	/*Stat = CreateDefaultSubobject<UStatComponent>(TEXT("Statcomp"));
//	Weapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weaponcomp"));
//
//	FName WeaponSocket(TEXT("hand_rSocket"));
//	Weapon->WeaponMesh->SetupAttachment(GetMesh(), WeaponSocket);*/
//}

ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleRadius(44.0f);



}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorRotation().Vector() * 1000, FColor::Blue, false, 1, 1, 10);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetControlRotation().Vector() * 1000, FColor::Red, false, 1, 1, 10);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

