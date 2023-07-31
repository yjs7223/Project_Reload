// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerMoveComponent.h"
#include "PlayerWeaponComponent.h"
#include "PlayerStatComponent.h"
#include "PlayerInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Player_HP_Widget.h"
#include "Player_Ammo_Widget.h"
#include "CoverComponent.h"
#include "Crosshair_Widget.h"
#include "Player_Cover_Widget.h"
#include "CameraControllComponent.h"
#include "Kismet/KismetMathLibrary.h"
//#include "Kismet/GameplayStatics.h"
//#include "Engine.h"
//#include "EngineMinimal.h"



APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sk_asset(TEXT("SkeletalMesh'/Game/CyberpunkMetalhead/Meshes/SKM_CyberpunkMetalhead_FullBodyA.SKM_CyberpunkMetalhead_FullBodyA'"));
	if (sk_asset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(sk_asset.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	/*const ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimObj(TEXT("AnimBlueprint'/Game/NewAnim/ABP_Charactor.ABP_Charactor'"));
	GetMesh()->SetAnimInstanceClass(AnimObj.Object->GeneratedClass);*/

	m_FollowSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("FollowSpringArm"));
	m_FollowSpringArm->SetupAttachment(RootComponent);
	m_FollowSpringArm->bUsePawnControlRotation = true;
	m_FollowSpringArm->TargetArmLength = 120.f;
	m_FollowSpringArm->SocketOffset = FVector(0, 60, 80);
	
	m_FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	m_FollowCamera->SetupAttachment(m_FollowSpringArm, USpringArmComponent::SocketName);

	stat = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("PlayerStat"));
	weapon = CreateDefaultSubobject<UPlayerWeaponComponent>(TEXT("PlayerWeapon"));
	FName WeaponSocket(TEXT("hand_r_Socket"));
	weapon->WeaponMesh->SetupAttachment(GetMesh(), WeaponSocket);
  
	m_PlayerMove = CreateDefaultSubobject<UPlayerMoveComponent>(TEXT("PlayerMove"));
	m_InputComponent = CreateDefaultSubobject<UPlayerInputComponent>(TEXT("InputComponent"));
	m_CameraControll = CreateDefaultSubobject<UCameraControllComponent>(TEXT("CameraControll"));
	m_CoverComponent = CreateDefaultSubobject<UCoverComponent>(TEXT("CoverComp"));

	HPWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerHP_Widget"));
	HPWidgetComponent->SetupAttachment(GetMesh(), TEXT("HP_Widget_Socket"));

	AmmoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerAmmo_Widget"));
	AmmoWidgetComponent->SetupAttachment(weapon->WeaponMesh, TEXT("AmmoWidgetSocket"));

	CoverWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerCover_Widget"));
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	InitWidget(nullptr, 0);
	GEngine->GameViewport->Viewport->ViewportResizedEvent.AddUObject(this, &APlayerCharacter::InitWidget);
	stat->SetHP(1000.0f);
	WidgetShow();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector coverpoint = FindComponentByClass<UCoverComponent>()->getCanCoverPoint();
	UPlayer_Cover_Widget* coverwidget = Cast<UPlayer_Cover_Widget>(CoverWidgetComponent->GetWidget());
	if (coverpoint != FVector::ZeroVector)
	{
		coverpoint.Z -= 80.0f;
		CoverWidgetComponent->SetWorldLocation(coverpoint);
		FRotator rot = FindComponentByClass<UCoverComponent>()->GetPointNormal().Rotation();
		CoverWidgetComponent->SetWorldRotation(rot);
		if (coverwidget) {
			coverwidget->SetOpacity(1.0f);
		}
		
	}
	else
	{
		if (coverwidget) {
			coverwidget->SetOpacity(0.0f);
		}

	}
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

void APlayerCharacter::InitWidget(FViewport* viewport, uint32 value)
{
	if (Crosshair_WidgetClass)
	{
		if (Crosshair_Widget)
		{
			Crosshair_Widget->RemoveFromViewport();
		}
		Crosshair_Widget = CreateWidget<UCrosshair_Widget>(Cast<APlayerController>(GetController()), Crosshair_WidgetClass);
		Crosshair_Widget->AddToViewport();
		//Crosshair_Widget->weapon = weapon;
	}

	if (HPWidgetComponent)
	{
		HPWidgetComponent->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));
		//HPWidgetComponent->SetupAttachment(GetMesh(), TEXT("HP_Widget_Socket"));
		HPWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
		HPWidgetComponent->SetDrawSize(FVector2D(160.0f, 160.0f));
		
		if (HP_Widget)
		{
			HPWidgetComponent->SetWidgetClass(HP_Widget);
			//Cast<UPlayer_HP_Widget>(HPWidgetComponent->GetWidget())->SetWidgetVisible();
			//Cast<UPlayer_HP_Widget>(HPWidgetComponent->GetWidget())->stat = stat;
		}
	}
	if (AmmoWidgetComponent)
	{
		AmmoWidgetComponent->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
		//AmmoWidgetComponent->SetupAttachment(weapon->WeaponMesh, TEXT("AmmoWidgetSocket"));
		AmmoWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
		AmmoWidgetComponent->SetDrawSize(FVector2D(130.0f, 50.0f));

		if (Ammo_Widget)
		{
			AmmoWidgetComponent->SetWidgetClass(Ammo_Widget);
			//Cast<UPlayer_Ammo_Widget>(AmmoWidgetComponent->GetWidget())->weapon = weapon;
		}
	}

	if (CoverWidgetComponent)
	{
		//CoverWidgetComponent->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
		//AmmoWidgetComponent->SetupAttachment(weapon->WeaponMesh, TEXT("AmmoWidgetSocket"));
		CoverWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
		CoverWidgetComponent->SetDrawSize(FVector2D(128.0f, 64.0f));

		if (Cover_Widget)
		{
			CoverWidgetComponent->SetWidgetClass(Cover_Widget);
		}
	}

}

void APlayerCharacter::UpdateWidget(float deltatime)
{
	if (HPWidgetComponent)
	{
		UPlayer_HP_Widget* hpw = Cast<UPlayer_HP_Widget>(HPWidgetComponent->GetWidget());
		if (hpw)
		{
			hpw->SetPercent(stat->curHP / stat->maxHP);
			hpw->MoveCircle(deltatime);
		}
	}

	if (Crosshair_Widget)
	{
		Crosshair_Widget->UpdateCrosshair(deltatime);
	}
}

void APlayerCharacter::WidgetShow()
{
  if (HPWidgetComponent) {
		UPlayer_HP_Widget* hpWidget = Cast<UPlayer_HP_Widget>(HPWidgetComponent->GetWidget());
		if (hpWidget) {
			hpWidget->SetWidgetVisible();
		}
	}
	if (Crosshair_Widget)
	{
		Crosshair_Widget->SetWidgetVisible();
	}
}


