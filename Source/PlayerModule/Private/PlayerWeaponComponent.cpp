// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeaponComponent.h"
#include "PlayerCharacter.h"
#include "Curves/CurveVector.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

UPlayerWeaponComponent::UPlayerWeaponComponent()
{

}

void UPlayerWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner<APlayerCharacter>();
	SetAmmo(300);
	m_recoilAmount = FVector2D::ZeroVector;
	m_recoveryAmount = FVector2D::ZeroVector;
	m_firecount = 0;
	m_turnValue = 0.f;
	m_lookupValue = 0.f;
	m_firerate = 0.2f; 
	m_dValue = 0.f;
	// ...

}


// Called every frame
void UPlayerWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bAddRecoil)
	{
		AddRecoil();
	}
	else
	{
		//Recovery();
	}

	if (bResetRecoil)
	{
		Recovery();
	}
	// ...
}

void UPlayerWeaponComponent::bindInput(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &UPlayerWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &UPlayerWeaponComponent::StopFire);

	PlayerInputComponent->BindAction("Aiming", IE_Pressed, this, &UPlayerWeaponComponent::StartAiming);
	PlayerInputComponent->BindAction("Aiming", IE_Released, this, &UPlayerWeaponComponent::StopAiming);

	//PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &UWeaponComponent::StartReload);
}

void UPlayerWeaponComponent::Fire()
{
	//owner
	FVector start;
	FRotator cameraRotation;
	FVector end;
	owner->Controller->GetPlayerViewPoint(start, cameraRotation);
	end = start + (cameraRotation.Vector() * 99999);

	FHitResult m_result = FHitResult();
	FCollisionQueryParams param(NAME_None, true, owner);

	DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 2.0f);
	if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_Visibility, param))
	{
		DrawDebugPoint(GetWorld(), m_result.Location, 10, FColor::Red, false, 2.f, 0);
		end = m_result.Location;
		start = WeaponMesh->GetSocketLocation(TEXT("SilencerMuzzleFlashSocket"));
		DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, 2.0f);
		if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_Visibility, param))
		{
			DrawDebugPoint(GetWorld(), m_result.Location, 10, FColor::Blue, false, 2.f, 0);
			isHit = true;
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, 2.0f);
		start = WeaponMesh->GetSocketLocation(TEXT("SilencerMuzzleFlashSocket"));
		if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_Visibility, param))
		{
			DrawDebugPoint(GetWorld(), m_result.Location, 10, FColor::Blue, false, 2.f, 0);
			isHit = true;
		}
	}
	bAddRecoil = true;
	m_firecount++;

}

void UPlayerWeaponComponent::StartAiming()
{
	owner->FollowSpringArm->TargetArmLength = 60.0f;
	isAiming = true;
}

void UPlayerWeaponComponent::StopAiming()
{
	owner->FollowSpringArm->TargetArmLength = 120.0f;
	isAiming = false;
}

void UPlayerWeaponComponent::StartFire()
{
	isFire = true;
	Fire();
	bResetRecoil = false;
	owner->GetWorldTimerManager().SetTimer(fHandle, this, &UPlayerWeaponComponent::Fire, m_firerate, true);
	//Fire();
}

void UPlayerWeaponComponent::StopFire()
{
	owner->GetWorldTimerManager().ClearTimer(fHandle);
	isFire = false;
	bResetRecoil = true;
}

void UPlayerWeaponComponent::StartReload()
{
	isReload = true;
}

void UPlayerWeaponComponent::AddRecoil()
{
	//m_turnValue = FMath::FRandRange(-2, 2);
	//m_lookupValue = FMath::FRandRange(-1, -2);
	if (m_dValue == 0.f)
	{
		m_turnValue = FMath::FRandRange(-1.0f, 1.0f);
		m_lookupValue = FMath::FRandRange(-1.f, -2.f);
		m_IturnValue += m_turnValue;
		m_IlookupValue += m_lookupValue;
	}
	m_dValue += 10.0f * GetWorld()->GetDeltaSeconds();
	float y = m_turnValue - FMath::Lerp(0.0f, m_turnValue, m_dValue);
	float p = m_lookupValue - FMath::Lerp(0.0f, m_lookupValue, m_dValue);
	//FMath::FInterpTo(0.0f, m_lookupValue, GetWorld()->GetDeltaSeconds(), 10.0f);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(m_dValue));
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(m_turnValue));
	/*if (m_firecount <= 5.0f)
	{
		owner->AddControllerYawInput(y * .2f);
		owner->AddControllerPitchInput(p * .2f);
	}*/
	owner->AddControllerYawInput(y * .1f);
	owner->AddControllerPitchInput(p * .1f);
	//m_IturnValue += y * .1f;
	//m_IlookupValue += p * .1f;
	if (m_dValue >= 1.0f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("end"));
		bAddRecoil = false;
		m_dValue = 0.f;
	}
	bUpdateRecoil = true;

}

void UPlayerWeaponComponent::Recovery()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("recovery"));
	m_dValue += 1.0f * GetWorld()->GetDeltaSeconds();
	float y = m_IturnValue - FMath::Lerp(0.0f, m_IturnValue, m_dValue); //FMath::FInterpTo(m_IturnValue, 0.0f, GetWorld()->GetDeltaSeconds(), 10.0f);
	float p = m_IlookupValue - FMath::Lerp(0.0f, m_IlookupValue, m_dValue); //FMath::FInterpTo(m_IlookupValue, 0.0f, GetWorld()->GetDeltaSeconds(), 10.0f);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(y));
	if (y > 0.0f)
	{
		owner->AddControllerYawInput(-(y) * 0.1f);
		owner->AddControllerPitchInput(-(p) * 0.1f);
	}
	else
	{
		bResetRecoil = false;
		m_IturnValue = 0;
		m_IlookupValue = 0;
	}
}


