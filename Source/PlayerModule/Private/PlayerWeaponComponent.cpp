// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeaponComponent.h"
#include "PlayerCharacter.h"

UPlayerWeaponComponent::UPlayerWeaponComponent()
{

}

void UPlayerWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner<APlayerCharacter>();
	SetAmmo(300);
	// ...

}


// Called every frame
void UPlayerWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerWeaponComponent::bindInput(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &UWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &UWeaponComponent::StopFire);

	//PlayerInputComponent->BindAction("Aiming", IE_Pressed, this, &UWeaponComponent::StartAiming);
	//PlayerInputComponent->BindAction("Aiming", IE_Released, this, &UWeaponComponent::StopAiming);

	//PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &UWeaponComponent::StartReload);
}

void UPlayerWeaponComponent::Fire()
{

}

