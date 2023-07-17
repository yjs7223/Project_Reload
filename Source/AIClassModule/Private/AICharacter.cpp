// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "AIWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "AICharacterMoveComponent.h"
#include "AIPatrolComponent.h"
#include "ST_Range.h"
#include "ST_Suppression.h"
#include "Animation/AnimInstance.h"
#include "Math/UnrealMathUtility.h"
#include "AISensingComponent.h"


AAICharacter::AAICharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AIMovement = CreateDefaultSubobject<UAICharacterMoveComponent>(TEXT("AIMovement"));
	AIWeapon = CreateDefaultSubobject<UAIWeaponComponent>(TEXT("AIWeapon"));
	AIPatrol = CreateDefaultSubobject<UAIPatrolComponent>(TEXT("AIPatrol"));
	AISensing = CreateDefaultSubobject<UAISensingComponent>(TEXT("AISensing"));
	

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

	CollisionMesh = CreateDefaultSubobject<UCapsuleComponent>(FName("CapSule")); //CreateDefaultSubobject<UCapsuleComponent>(FName("CapSule"));
	CollisionMesh->SetupAttachment(RootComponent);
	//RootComponent = CollisionMesh;

	CollisionMesh->SetCapsuleRadius(sup_HitRadius);
	CollisionMesh->SetCapsuleHalfHeight(sup_HitHeight);
	//CollisionMesh->SetRelativeLocation(FVector(0, 0, 0));
	
	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnOverlapBegin);

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

		sup_HitRadius = RangeData->Sup_HitRadius;
		sup_HitHeight = RangeData->Sup_HitHeight;
	}
	
}

void AAICharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AAICharacter::IdleAnim()
{
	// ���κ� ���� ������� (�ִϸ��̼� ������ �ҵ�)
	//PlayAnimMontage(idle_Montage, 1.0f);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Play")));
}

