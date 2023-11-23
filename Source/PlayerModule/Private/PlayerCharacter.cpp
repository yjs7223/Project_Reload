// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerMoveComponent.h"
#include "PlayerWeaponComponent.h"
#include "PlayerStatComponent.h"
#include "PlayerInputComponent.h"
#include "Components/SceneComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Player_HP_Widget.h"
#include "Player_Ammo_Widget.h"
#include "CoverComponent.h"
#include "PlayerCharactorController.h"
#include "Damage_Widget.h"
#include "PlayerHUDWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CharacterSoundDataAsset.h"
#include "Kismet/KismetMaterialLibrary.h"

//#include "Kismet/GameplayStatics.h"
//#include "Engine.h"
//#include "EngineMinimal.h"



APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sk_asset(TEXT("SkeletalMesh'/Game/Characters/CyberpunkMetalhead/Meshes/SKM_CyberpunkMetalhead_FullBodyA.SKM_CyberpunkMetalhead_FullBodyA'"));
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
	m_FollowSpringArm->SetupAttachment(GetMesh(), TEXT("root"));

	//드론 스프링 컴포넌트 추가 부분 CreateDefaultSubobject<USceneComponent>(TEXT("Empcmp"));
	m_DroneScene = CreateDefaultSubobject<USceneComponent>(TEXT("DroneScene"));
	m_DroneScene->SetupAttachment(GetMesh(), TEXT("root"));
	m_DroneScene->SetRelativeLocation(FVector(40, 35, 200));


	m_DroneSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("DroneSpringArm"));
	m_DroneSpringArm->bUsePawnControlRotation = true;
	m_DroneSpringArm->SetupAttachment(m_DroneScene);
	m_DroneSpringArm->bDoCollisionTest = false;
	m_DroneSpringArm->bEnableCameraLag = true;
	m_DroneSpringArm->bEnableCameraRotationLag = true;
	m_DroneSpringArm->CameraLagSpeed = 3.0f;
	m_DroneSpringArm->CameraRotationLagSpeed = 1;
	m_DroneSpringArm->bClampToMaxPhysicsDeltaTime = true;
	m_DroneSpringArm->TargetArmLength = 10;






	m_FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	m_FollowCamera->SetupAttachment(m_FollowSpringArm, USpringArmComponent::SocketName);

	stat = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("PlayerStat"));
	weapon = CreateDefaultSubobject<UPlayerWeaponComponent>(TEXT("PlayerWeapon"));

	FName WeaponSocket(TEXT("hand_r_Socket"));
	weapon->WeaponMesh->SetupAttachment(GetMesh(), WeaponSocket);
  
	m_PlayerMove = CreateDefaultSubobject<UPlayerMoveComponent>(TEXT("PlayerMove"));
	m_CoverComponent = CreateDefaultSubobject<UCoverComponent>(TEXT("CoverComp"));
	InputComponent = CreateDefaultSubobject<UPlayerInputComponent>(InputComponentName);


	/*HPWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerHP_Widget"));
	HPWidgetComponent->SetupAttachment(GetMesh(), TEXT("HP_Widget_Socket"));

	HPWidgetComponent_back = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerHP_Widget_back"));
	HPWidgetComponent_back->SetupAttachment(GetMesh(), TEXT("HP_Widget_Socket"));

	AmmoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerAmmo_Widget"));
	AmmoWidgetComponent->SetupAttachment(weapon->WeaponMesh, TEXT("AmmoWidgetSocket"));

	CoverWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerCover_Widget"));*/
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
	auto temp = [this](UMeshComponent* mesh) {
		for (int32 i = 0; i < mesh->GetNumMaterials(); ++i)
		{
			UMaterialInterface* MaterialInterface = mesh->GetMaterial(i);
			if (!MaterialInterface) continue;

			UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(MaterialInterface);
			if (!DynamicMaterial) {
				DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
				mesh->SetMaterial(i, DynamicMaterial);
			}

			if (DynamicMaterial) {
				DynamicMaterial->SetScalarParameterValue("OpacityMaskValue", 0.0f);
			}
		}

		};
	//temp(GetMesh());
	//temp(weapon->WeaponMesh);

	/*FVector DroneDefLoc = FVector(40, 35, 200);
	m_DroneSpringArm->SetRelativeLocation(FVector(40, 35, 200));*/
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
	if (PlayerHUDWidgetClass)
	{
		if (PlayerHUDWidget)
		{
			PlayerHUDWidget->RemoveFromViewport();
		}
		PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(Cast<APlayerController>(GetController()), PlayerHUDWidgetClass);
		if (PlayerHUDWidget) {
			PlayerHUDWidget->AddToViewport();
		}
	}
}

void APlayerCharacter::UpdateWidget(float deltatime)
{
	//if (HPWidgetComponent)
	//{
	//	float yaw = GetControlRotation().Yaw;// * 0.3f;
	//	FRotator widrot = HPWidgetComponent->GetRelativeRotation();
	//	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(yaw));
	//	//widrot.Yaw *= 0.3f;
	//	widrot.Yaw = (yaw + 180);// * 0.3f;//+60;
	//	widrot.Yaw *= 0.3f;
	//	//widrot.Yaw -= 120;
	//	HPWidgetComponent->SetRelativeRotation(widrot);
	//}
}

void APlayerCharacter::WidgetShow()
{
	OnVisibleAllUIDelegate.Broadcast();
}


