// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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

void UWeaponComponent::SetAmmo(int p_ammo)
{
	maxAmmo = p_ammo;
	curAmmo = p_ammo;
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
	return;
}

