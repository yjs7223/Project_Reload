// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "Components/InputComponent.h"
#include "BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "CoverComponent.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	m_CanShooting = true;
	Weapon_Handle_R_Name = TEXT("hand_r_Socket");
	Weapon_Handle_L_Name = TEXT("hand_l_Socket");

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sk_rifle(TEXT("SkeletalMesh'/Game/ThirdPersonKit/Meshes/WeaponsTPSKitOrginals/Rifle/SKM_Rifle_01.SKM_Rifle_01'"));
	if (sk_rifle.Succeeded())
	{
		RifleMesh = sk_rifle.Object;
		WeaponMesh->SetSkeletalMesh(RifleMesh);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sk_pistol(TEXT("SkeletalMesh'/Game/ThirdPersonKit/Meshes/WeaponsTPSKitOrginals/PIstolScifi/SKM_PistolSciFi.SKM_PistolSciFi'"));
	if (sk_pistol.Succeeded())
	{
		PistolMesh = sk_pistol.Object;
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sk_shotgun(TEXT("SkeletalMesh'/Game/ThirdPersonKit/Meshes/WeaponsTPSKitOrginals/Shotgun/SKM_Shotgun.SKM_Shotgun'"));
	if (sk_shotgun.Succeeded())
	{
		ShotgunMesh = sk_shotgun.Object;
	}
	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	AimSetting();
	// ...
}

//void UWeaponComponent::bindInput(UInputComponent* PlayerInputComponent)
//{
//	return;
//}

void UWeaponComponent::SetAmmo(int p_ammo)
{
	holdAmmo = p_ammo;
	switch (weapontype)
	{
	case EWeaponType::TE_Pistol:
		curAmmo = 10;
		break;
	case EWeaponType::TE_Rifle:
		curAmmo = 30;
		break;
	case EWeaponType::TE_Shotgun:
		curAmmo = 7;
		break;
	default:
		curAmmo = 30;
		break;
	}
}

void UWeaponComponent::ReloadAmmo()
{
	if (holdAmmo <= 0)
	{
		return;
	}

	if (curAmmo >= 30)
	{
		return;
	}


	int m_ammo = 0;
	if (holdAmmo < 30)
	{
		m_ammo = holdAmmo;
	}
	else
	{
		m_ammo = 30;
		holdAmmo -= m_ammo;
	}

	isReload = true;
	curAmmo += m_ammo;
}

void UWeaponComponent::Fire()
{
	//fire
	return;
}

float UWeaponComponent::getAimYaw()
{
	return aimOffset.Yaw;
}

float UWeaponComponent::getAimPitch()
{
	return aimOffset.Pitch;
}

void UWeaponComponent::AimSetting()
{
	FRotator temprot;
	ACharacter* Owner = GetOwner<ACharacter>();
	temprot = Owner->GetControlRotation() - Owner->GetActorRotation();

	aimOffset.Pitch = FMath::ClampAngle(temprot.Pitch, -90, 90);
	aimOffset.Yaw = temprot.Yaw;
	if (aimOffset.Yaw > 180) aimOffset.Yaw -= 360;

	FRotator cameraRotation;
	FVector start;
	Owner->Controller->GetPlayerViewPoint(start, cameraRotation);
	FVector end = start + (cameraRotation.Vector() * 99999);

	FHitResult result;
	FCollisionQueryParams param(NAME_None, true, GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param))
	{
		temprot = UKismetMathLibrary::FindLookAtRotation(result.Location, start);
	}
	else
	{
		temprot = UKismetMathLibrary::FindLookAtRotation(end, start);
	}


}

void UWeaponComponent::SetHandleing(bool isFaceRight, bool isCoverUse)
{
	FName handSocketName;
	FRotator meshRotate;
	FVector meshLocation;
	ACharacter* character = GetOwner<ACharacter>();
	if (isCoverUse) {
		if (UCoverComponent* cover = character->FindComponentByClass<UCoverComponent>()) {
			isFaceRight = cover->IsFaceRight();
		}
	}
	if (isFaceRight) {
		handSocketName = Weapon_Handle_R_Name;
		meshRotate = FRotator(0.0, 0.0, 0.0);
		meshLocation = WeaponMesh->GetRelativeLocation() * FVector(-1.0f, 1.0f, -1.0f);
	}
	else {
		handSocketName = Weapon_Handle_L_Name;
		meshRotate = FRotator(0.0, 180.0, 180.0);
		meshLocation = WeaponMesh->GetRelativeLocation() * FVector(-1.0f, 1.0f, -1.0f);
	}


	WeaponMesh->AttachToComponent(GetOwner<ACharacter>()->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, handSocketName);
	WeaponMesh->SetRelativeRotation(meshRotate);
	WeaponMesh->SetRelativeLocation(meshLocation);
}

