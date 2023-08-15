
// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "AI_Controller.h"
#include "AICommander.h"
#include "AIWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "AICharacterMoveComponent.h"
#include "AIPatrolComponent.h"
#include "ST_Range.h"
#include "ST_Suppression.h"
#include "ST_AIBaseStat.h"
#include "Animation/AnimInstance.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Vector2D.h"
#include "AISensingComponent.h"
#include "LastPoint.h"
#include "AISpawner.h"
#include "AIStatComponent.h"
#include "AIInputComponent.h"
#include "CoverComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI_HP_Widget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIPatrolComponent.h"
#include "AISensingComponent.h"
#include "AIWeaponComponent.h"
#include "AIInputComponent.h"
#include "CoverComponent.h"
#include "AIStatComponent.h"
#include "AICharacterMoveComponent.h"

AAICharacter::AAICharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AIMovement = CreateDefaultSubobject<UAICharacterMoveComponent>(TEXT("AIMovement"));
	AIWeapon = CreateDefaultSubobject<UAIWeaponComponent>(TEXT("AIWeapon"));
	AIPatrol = CreateDefaultSubobject<UAIPatrolComponent>(TEXT("AIPatrol"));
	AISensing = CreateDefaultSubobject<UAISensingComponent>(TEXT("AISensing"));
	AIStat = CreateDefaultSubobject<UAIStatComponent>(TEXT("AIStat"));
	m_InputComponent = CreateDefaultSubobject<UAIInputComponent>(TEXT("InputComponent"));
	m_CoverComponent = CreateDefaultSubobject<UCoverComponent>(TEXT("CoverComp"));
	AIControllerClass = AAI_Controller::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sk_asset(TEXT("SkeletalMesh'/Game/Animation/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (sk_asset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(sk_asset.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	FName WeaponSocket(TEXT("hand_r_Socket"));
	AIWeapon->WeaponMesh->SetupAttachment(GetMesh(), WeaponSocket);

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_RangeDataObject(TEXT("DataTable'/Game/AI_Project/DT/DT_Range.DT_Range'"));
	if (DT_RangeDataObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
		DT_Range = DT_RangeDataObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SuppressionDataObject(TEXT("DataTable'/Game/AI_Project/DT/DT_Suppression.DT_Suppression'"));
	if (DT_SuppressionDataObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
		DT_Suppression = DT_SuppressionDataObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_AIBaseStatDataObject(TEXT("DataTable'/Game/AI_Project/DT/DT_AIBaseStat.DT_AIBaseStat'"));
	if (DT_AIBaseStatDataObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
		DT_AIBaseStat = DT_AIBaseStatDataObject.Object;
	}

	ConstructorHelpers::FObjectFinder<UBlueprint> GrenadeData(TEXT("Blueprint'/Game/Aws/BP_GrenadeDummy.BP_GrenadeDummy'"));
	if (GrenadeData.Succeeded())
	{
		GrenadeBlueprint = (UClass*)GrenadeData.Object->GeneratedClass;
	}

	CollisionMesh = CreateDefaultSubobject<UCapsuleComponent>(FName("CapSule")); //CreateDefaultSubobject<UCapsuleComponent>(FName("CapSule"));
	CollisionMesh->SetupAttachment(RootComponent);
	//RootComponent = CollisionMesh;

	CollisionMesh->SetCapsuleRadius(sup_HitRadius);
	CollisionMesh->SetCapsuleHalfHeight(sup_HitHeight);
	//CollisionMesh->SetRelativeLocation(FVector(0, 0, 0));
	
	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnOverlapBegin);


	HPWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerHP_Widget"));
	HPWidgetComponent->SetupAttachment(GetMesh());
	HPWidgetComponent->SetRelativeLocation(FVector(.0f, .0f, 210.0f));

}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();


	mesh = FindComponentByClass<USkeletalMeshComponent>();

	switch (type)
	{
	case Enemy_Name::RIFLE:
		SetDataTable("Rifle_E");
		break;
	case Enemy_Name::HEAVY:
		SetDataTable("Heavy_E");
		break;
	case Enemy_Name::SNIPER:
		SetDataTable("Sniper_E");
		break;
	}

	if (AIStat)
	{
		AIStat->SetHP(ai_HP); ////
		Cast<AAI_Controller>(GetController())->GetBlackboardComponent()->SetValueAsFloat("AI_HP", AIStat->maxHP);
	}
	if (!player)
	{
		player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}
	
	/*if (!commander)
	{
		commander = Cast<AAICommander>(UGameplayStatics::GetActorOfClass(GetWorld(), AAICommander::StaticClass()));
	}*/

	InitWidget();

	
	// Init
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);

	AIPatrol->SetComponentTickEnabled(false);
	AISensing->SetComponentTickEnabled(false);
	AIMovement->SetComponentTickEnabled(false);
	AIWeapon->SetComponentTickEnabled(false);
	AIStat->SetComponentTickEnabled(false);
	m_InputComponent->SetComponentTickEnabled(false);
	m_CoverComponent->SetComponentTickEnabled(false);
	//
}

void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateWidget();
}



void AAICharacter::InitWidget()
{
	if (HPWidgetComponent)
	{
		HPWidgetComponent->SetWorldScale3D(FVector(0.3f, 0.3f, 0.3f));
		HPWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
		HPWidgetComponent->SetDrawSize(FVector2D(600.0f, 100.0f));

		if (HP_Widget)
		{
			HPWidgetComponent->SetWidgetClass(HP_Widget);
			Cast<UAI_HP_Widget>(HPWidgetComponent->GetWidget())->SetDelegate(this);
		}
	}
}

void AAICharacter::UpdateWidget()
{
	if (player)
	{
		FRotator m_rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), player->GetActorLocation());
		HPWidgetComponent->SetWorldRotation(FRotator(0, m_rot.Yaw, 0));
	}
	else
	{	
		player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}
}

void AAICharacter::SetDataTable(FName EnemyName)
{
	AIMovement->SetEnemy(EnemyName);
	AIWeapon->SetDataTable(EnemyName);
	AIStat->SetDataTable(EnemyName);
	AISensing->SetDataTable(EnemyName);
	FST_Range* RangeData = DT_Range->FindRow<FST_Range>(EnemyName, FString(""));
	if (RangeData)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData Succeed!"));

		sup_HitRadius = RangeData->Sup_HitRadius;
		sup_HitHeight = RangeData->Sup_HitHeight;
	}

	FST_Suppression* SuppressionData = DT_Suppression->FindRow<FST_Suppression>(EnemyName, FString(""));
	if (SuppressionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData Succeed!"));

		sup_sharerange = SuppressionData->Sup_ShareRange;
		sup_sharetime = SuppressionData->Sup_ShareTime;
	}

	FST_AIBaseStat* AIBaseStatData = DT_AIBaseStat->FindRow<FST_AIBaseStat>(EnemyName, FString(""));
	if (AIBaseStatData)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData Succeed!"));

		ai_HP = AIBaseStatData->AI_HP;
	}

	
}

void AAICharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AAICharacter::IdleAnim()
{
	//PlayAnimMontage(idle_Montage, 1.0f);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Play")));
}

void AAICharacter::FireInTheHole(AActor* myai,float Velocity)
{
	float Gravity = 980.0f;
	float Length_PlayerAI_XY = FVector2D((player->GetActorLocation().X - myai->GetActorLocation().X), (player->GetActorLocation().Y - myai->GetActorLocation().Y)).Length();
	float M_PlayerAI_Z = player->GetActorLocation().Z - myai->GetActorLocation().Z;
	float Gra_Leng = (Length_PlayerAI_XY * Length_PlayerAI_XY) * Gravity;
	float Velocity_Z = (M_PlayerAI_Z * (Velocity * Velocity)) * 2;
	float FourthSquare_Velocity = Velocity * Velocity * Velocity * Velocity;
	float M_Velocity = FourthSquare_Velocity - ((Gra_Leng + Velocity_Z) * Gravity);
	float D_Gra_Leng = 1 / (Length_PlayerAI_XY*Gravity);
	FRotator Find_rotator = UKismetMathLibrary::FindLookAtRotation( myai->GetActorLocation(), player->GetActorLocation());
	FRotator rotator = FRotator::ZeroRotator;
	rotator.Roll = Find_rotator.Roll;
	rotator.Yaw = Find_rotator.Yaw;
	rotator.Pitch = UKismetMathLibrary::DegAtan((sqrt(M_Velocity) + (Velocity * Velocity)) * D_Gra_Leng);
	if (M_Velocity >= 0)
	{
		GetWorld()->SpawnActor<AActor>(GrenadeBlueprint, myai->GetActorLocation(), rotator);
	}

}

void AAICharacter::Init()
{
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);

	AIPatrol->SetComponentTickEnabled(true);
	AISensing->SetComponentTickEnabled(true);
	AIMovement->SetComponentTickEnabled(true);
	AIWeapon->SetComponentTickEnabled(true);
	AIStat->SetComponentTickEnabled(true);
	m_InputComponent->SetComponentTickEnabled(true);
	m_CoverComponent->SetComponentTickEnabled(true);
}

void AAICharacter::Dead()
{
	SetActorTickEnabled(false);

	AIPatrol->SetComponentTickEnabled(false);

	AISensing->SetComponentTickEnabled(false);
	AISensing->GetOwner()->GetWorldTimerManager().ClearTimer(AISensing->sensingTimer);

	AIMovement->SetComponentTickEnabled(false);

	AIWeapon->SetComponentTickEnabled(false);
	AIWeapon->GetOwner()->GetWorldTimerManager().ClearTimer(AIWeapon->timer);

	AIStat->SetComponentTickEnabled(false);
	m_InputComponent->SetComponentTickEnabled(false);
	m_CoverComponent->SetComponentTickEnabled(false);
}


