// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "Components/InputComponent.h"
#include "BaseCharacter.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sk_weapon(TEXT("SkeletalMesh'/Game/ThirdPersonKit/Meshes/WeaponsTPSKitOrginals/Rifle/SKM_Rifle_01.SKM_Rifle_01'"));
	if (sk_weapon.Succeeded())
	{
		WeaponMesh->SetSkeletalMesh(sk_weapon.Object);
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

	// ...
}

//void UWeaponComponent::bindInput(UInputComponent* PlayerInputComponent)
//{
//	return;
//}

void UWeaponComponent::SetAmmo(int p_ammo)
{
	maxAmmo = p_ammo;
	curAmmo = 30;
}

void UWeaponComponent::ReloadAmmo()
{
	if (maxAmmo <= 0)
	{
		return;
	}

	if (curAmmo >= 30)
	{
		return;
	}


	int m_ammo = 0;
	if (maxAmmo < 30)
	{
		m_ammo = maxAmmo;
	}
	else
	{
		m_ammo = 30;
		maxAmmo -= m_ammo;
	}

	isReload = true;
	curAmmo += m_ammo;
}

void UWeaponComponent::Fire()
{
	//fire
	return;
}

