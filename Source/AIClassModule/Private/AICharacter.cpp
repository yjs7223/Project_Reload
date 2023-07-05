// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "AIWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "AICharacterMoveComponent.h"
#include "ST_Range.h"

AAICharacter::AAICharacter()
{
	AIMovement = CreateDefaultSubobject<UAICharacterMoveComponent>(TEXT("AIMovement"));
	AIWeapon = CreateDefaultSubobject<UAIWeaponComponent>(TEXT("AIWeapon"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sk_asset(TEXT("SkeletalMesh'/Game/Animation/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (sk_asset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(sk_asset.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	FName WeaponSocket(TEXT("hand_r_Socket"));
	AIWeapon->WeaponMesh->SetupAttachment(GetMesh(), WeaponSocket);

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_RangeDataObject(TEXT("DataTable'/Game/Aws/AI_Stat/DT_Range.DT_Range'"));
	if (DT_RangeDataObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
		DT_Range = DT_RangeDataObject.Object;
	}
	SetDataTable("Rifle_E");

	CollisionMesh = CreateDefaultSubobject<UCapsuleComponent>(FName("CapSule"));
	CollisionMesh->SetCapsuleRadius(HitRadius);
	CollisionMesh->SetCapsuleHalfHeight(HitHeight);
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAICharacter::SetDataTable(FName EnemyName)
{
	FST_Range* RangeData = DT_Range->FindRow<FST_Range>(EnemyName, FString(""));
	if (RangeData)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData Succeed!"));

		HitRadius = RangeData->Sup_HitRadius;
		HitHeight = RangeData->Sup_HitHeight;
		
	}
}
