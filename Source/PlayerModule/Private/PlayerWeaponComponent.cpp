// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeaponComponent.h"
#include "PlayerCharacter.h"
#include "Curves/CurveVector.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
//#include "RecoilComponent.h"

UPlayerWeaponComponent::UPlayerWeaponComponent()
{
	//recoil = CreateDefaultSubobject<URecoilHelper>(TEXT("Recoil"));
}

void UPlayerWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner<APlayerCharacter>();
	SetAmmo(300);
	m_firecount = 0;
	m_turnValue = 0.f;
	m_lookupValue = 0.f;
	m_firerate = 0.2f; 
	m_dValue = 0.f;
	recoilTime = 0.0f;
	yawRange = FVector2D(-0.3f, 0.3f);
	pitchRange = FVector2D(-0.3f, -1.0f);
	tt = 0;
	pp = 0;
	// ...

}


// Called every frame
void UPlayerWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RecoilTick(DeltaTime);
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
	// StopRecoil();
	FVector start;
	FRotator cameraRotation;
	FVector end;
	owner->Controller->GetPlayerViewPoint(start, cameraRotation);
	end = start + (cameraRotation.Vector() * 99999);
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("fire"));
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
	m_firecount++;
	StartRecoil();
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
	//PlayerStartRot = owner->GetController()->GetControlRotation();
	Fire();
	owner->GetWorldTimerManager().SetTimer(fHandle, this, &UPlayerWeaponComponent::Fire, m_firerate, true);
	//Fire();
}

void UPlayerWeaponComponent::StopFire()
{
	owner->GetWorldTimerManager().ClearTimer(fHandle);
	isFire = false;
	m_firecount = 0;
}

void UPlayerWeaponComponent::StartReload()
{
	isReload = true;
}

void UPlayerWeaponComponent::RecoilTick(float p_deltatime)
{
	if (bRecoil)
	{
		recoilTime += p_deltatime * 12.0f;
		float a = recoilTime;
		float b = easeOutExpo(a, 0, 0.45f, 1.0f);
		tt += 1;
		float y = FMath::Lerp(0.0f, yawRecoilValue, b)/tt;	
		float p = FMath::Lerp(0.0f, pitchRecoilValue, b)/tt;
		pp += p;
		owner->AddControllerYawInput(y);
		owner->AddControllerPitchInput(p);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(pp));

		if (a >= 1)
		{
			StopRecoil();
			recoilTime = 0;
			pp = 0;
			tt = 0;
		}

	}
	/*else if (bRecovery)
	{
		recoilTime = 0.0f;

		PlayerNowRot = owner->GetController()->GetControlRotation();
		recoveryTime += p_deltatime * 1.0f;
		FRotator r = FMath::Lerp<FRotator>(PlayerNowRot, PlayerStartRot, recoveryTime);
		FRotator yr = FRotator(r.Pitch, PlayerNowRot.Yaw, PlayerNowRot.Roll);
		owner->Controller->SetControlRotation(yr);
		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::SanitizeFloat(recoveryTime));
		
		if (recoveryTime >= 1.0f)
		{
			bRecovery = false;
			recoveryTime = 1.0f;
		}
		if (PlayerNowRot == PlayerStartRot)
		{
			bRecovery = false;
		}
	}
	else
	{
		recoveryTime = 0;
	}*/
}

void UPlayerWeaponComponent::StartRecoil()
{
	bRecoil = true;
	yawRecoilValue = FMath::RandRange(yawRange.X, yawRange.Y);
	pitchRecoilValue = FMath::RandRange(pitchRange.X, pitchRange.Y);
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::SanitizeFloat(yawRecoilValue));
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::SanitizeFloat(pitchRecoilValue));
}

void UPlayerWeaponComponent::StopRecoil()
{
	bRecoil = false;
	//StartRecovery();
}

void UPlayerWeaponComponent::StartRecovery()
{
	//bRecovery = true;
	//PlayerNowRot = owner->GetController()->GetControlRotation();
}

float UPlayerWeaponComponent::EasingOut(float Param)
{
	return FMath::Sin(-13.f * (PI / 2.f) * (Param + 1)) * FMath::Pow(2.f, -10.f * Param) + 1.f;
}

float UPlayerWeaponComponent::easeOutExpo(float t, float b, float c, float d)
{
	return (t == d) ? b + c : c * (-pow(2, -10 * t / d) + 1) + b;
}



