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
#include "Attacked_Widget.h"
#include "Damage_Widget.h"
#include "LineNaviWidget.h"
#include "CameraControllComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "CharacterSoundDataAsset.h"

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

	static ConstructorHelpers::FObjectFinder<UCharacterSoundDataAsset> CharSound(TEXT("CharacterSoundDataAsset'/Game/yjs/DA_CharacterSound.DA_CharacterSound'"));
	if (CharSound.Succeeded())
	{
		CharacterSound = CharSound.Object;
	}
	
	//const ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimObj(TEXT("AnimBlueprint'/Game/NewAnim/ABP_Charactor.ABP_Charactor'"));
	//GetMesh()->SetAnimInstanceClass(AnimObj.Object->GetAnimBlueprintGeneratedClass());
	//AnimObj.Object.anim
	//GetMesh().setanimins

	m_FollowSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("FollowSpringArm"));
	//m_FollowSpringArm->SetupAttachment(RootComponent);
	m_FollowSpringArm->bUsePawnControlRotation = true;
	m_FollowSpringArm->TargetArmLength = 120.f;
	m_FollowSpringArm->SocketOffset = FVector(0, 60, 80);
	m_FollowSpringArm->SetupAttachment(GetMesh(), TEXT("root"));
	
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

	HPWidgetComponent_back = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerHP_Widget_back"));
	HPWidgetComponent_back->SetupAttachment(GetMesh(), TEXT("HP_Widget_Socket"));

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
	FVector coverpoint = m_CoverComponent->getCanCoverPoint();
	FVector coverNormal = m_CoverComponent->GetPointNormal();
	UCapsuleComponent* capsule = GetCapsuleComponent();

	UPlayer_Cover_Widget* coverwidget = Cast<UPlayer_Cover_Widget>(CoverWidgetComponent->GetWidget());
	if (coverpoint != FVector::ZeroVector)
	{
		coverpoint -= coverNormal * capsule->GetScaledCapsuleRadius() * 0.5f;
		//coverpoint.Y -= 30.0f;
		CoverWidgetComponent->SetWorldLocation(coverpoint);
		FRotator rot = coverNormal.Rotation();
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
	//UpdateWidget(DeltaTime);
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
		if (Crosshair_Widget) {
			Crosshair_Widget->AddToViewport();
		}
		//Crosshair_Widget->weapon = weapon;
	}
	if (Attacked_WidgetClass)
	{
		if (Attacked_Widget)
		{
			Attacked_Widget->RemoveFromViewport();
		}
		Attacked_Widget = CreateWidget<UAttacked_Widget>(Cast<APlayerController>(GetController()), Attacked_WidgetClass);
		if (Attacked_Widget) {
			Attacked_Widget->AddToViewport();
		}
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

	if (HPWidgetComponent_back)
	{
		HPWidgetComponent_back->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));
		//HPWidgetComponent->SetupAttachment(GetMesh(), TEXT("HP_Widget_Socket"));
		HPWidgetComponent_back->SetWidgetSpace(EWidgetSpace::World);
		HPWidgetComponent_back->SetDrawSize(FVector2D(160.0f, 160.0f));
		HPWidgetComponent_back->SetRelativeRotation(FRotator(0, 180, 0));

		if (HP_Widget)
		{
			HPWidgetComponent_back->SetWidgetClass(HP_Widget);
			Cast<UPlayer_HP_Widget>(HPWidgetComponent_back->GetWidget())->SetBackMat();
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
		CoverWidgetComponent->SetDrawSize(FVector2D(64.0f, 64.0f));

		if (Cover_WidgetClass)
		{
			CoverWidgetComponent->SetWidgetClass(Cover_WidgetClass);
		}
	}

	if (!LineNavi_Widget)
	{
		LineNavi_Widget = CreateWidget<ULineNaviWidget>(Cast<APlayerController>(GetController()), ULineNaviWidget::StaticClass());
		LineNavi_Widget->AddToViewport();
	}

}

void APlayerCharacter::UpdateWidget(float deltatime)
{
	if (HPWidgetComponent)
	{
		float yaw = GetControlRotation().Yaw;// * 0.3f;
		FRotator widrot = HPWidgetComponent->GetRelativeRotation();
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(yaw));
		//widrot.Yaw *= 0.3f;
		widrot.Yaw = (yaw + 180);// * 0.3f;//+60;
		widrot.Yaw *= 0.3f;
		//widrot.Yaw -= 120;
		HPWidgetComponent->SetRelativeRotation(widrot);
	}
}

void APlayerCharacter::WidgetShow()
{
	OnVisibleAllUIDelegate.Broadcast();
}

void APlayerCharacter::CreateDamageWidget(float value, FHitResult result)
{
	if (Damage_WidgetClass)
	{
		/*UWidgetComponent* DWidgetComponent = NewObject<UWidgetComponent>(this);
		DWidgetComponent->SetWidgetClass(Damage_Widget);
		DWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
		DWidgetComponent->SetWorldLocation(result.Location);*/
		UDamage_Widget* dwidget = CreateWidget<UDamage_Widget>(Cast<APlayerController>(GetController()), Damage_WidgetClass);
		if (dwidget)
		{
			dwidget->SetDamageText(value, result);
			dwidget->AddToViewport();
		}
	}
}


