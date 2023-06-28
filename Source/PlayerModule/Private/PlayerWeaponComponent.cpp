// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeaponComponent.h"
#include "PlayerCharacter.h"
#include "Curves/CurveVector.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"
#include "BaseInputComponent.h"
#include "Particles/ParticleSystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetStringLibrary.h"

UPlayerWeaponComponent::UPlayerWeaponComponent()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/yjs/PlayerWeaponData.PlayerWeaponData'"));
	if (DataTable.Succeeded())
	{
		PlayerWeaponData = DataTable.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Fire(TEXT("ParticleSystem'/Game/ThirdPersonKit/Particles/P_RealAssaultRifle_MF.P_AssaultRifle_MF'"));
	if (Fire.Succeeded())
	{
		MuzzleFireParticle = Fire.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> bullet(TEXT("ParticleSystem'/Game/ThirdPersonKit/Particles/P_BulletTracer.P_BulletTracer'"));
	if (bullet.Succeeded())
	{
		BulletTracerParticle = bullet.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> decal(TEXT("MaterialInstanceConstant'/Game/yjs/Impact_Decal2.Impact_Decal2'"));
	if (decal.Succeeded())
	{
		Decal = decal.Object;
	}


}

void UPlayerWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner<APlayerCharacter>();
	SetAmmo(300);
	m_firecount = 0;
	m_turnValue = 0.f;
	m_lookupValue = 0.f;
	m_firerate = 0.1f; 
	m_dValue = 0.f;
	recoilTime = 0.0f;
	m_spreadPower = 3.0f;
	yawRange = FVector2D(-0.3f, 0.3f);
	pitchRange = FVector2D(-0.3f, -1.0f);
	TickCount = 1;
	pp = 0;
	// ...

}


// Called every frame
void UPlayerWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	RecoilTick(DeltaTime);
	//RecoveryTick(DeltaTime);
}

void UPlayerWeaponComponent::Fire()
{
	FVector start;
	FRotator cameraRotation;
	FVector end;
	owner->Controller->GetPlayerViewPoint(start, cameraRotation);
	float spread = m_firecount * m_spreadPower;
	start.X += FMath::RandRange(-spread, spread);
	start.Y += FMath::RandRange(-spread, spread);
	end = start + (cameraRotation.Vector() * 99999);
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("fire"));
	FHitResult m_result = FHitResult();
	FCollisionQueryParams param(NAME_None, true, owner);
	FRotator m_rot;
	GameStatic->SpawnEmitterAttached(MuzzleFireParticle, WeaponMesh, FName("MuzzleFlashSocket"));

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
			m_rot = UKismetMathLibrary::FindLookAtRotation(start, m_result.Location);
			SpawnDecal(m_result);
			isHit = true;
		}
		else
		{
			m_rot = UKismetMathLibrary::FindLookAtRotation(start, end);
			SpawnDecal(m_result);
		}
		GameStatic->SpawnEmitterAttached(BulletTracerParticle, WeaponMesh, FName("MuzzleFlashSocket"));
	}
	else
	{
		DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, 2.0f);
		start = WeaponMesh->GetSocketLocation(TEXT("SilencerMuzzleFlashSocket"));
		if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_Visibility, param))
		{
			DrawDebugPoint(GetWorld(), m_result.Location, 10, FColor::Blue, false, 2.f, 0);
			m_rot = UKismetMathLibrary::FindLookAtRotation(start, m_result.Location);
			SpawnDecal(m_result);
			isHit = true;
		}
		else
		{
			m_rot = UKismetMathLibrary::FindLookAtRotation(start, end);
		}
		GameStatic->SpawnEmitterAttached(BulletTracerParticle, WeaponMesh, FName("MuzzleFlashSocket"));
	}
	if(m_firecount < 10)
	{
		m_firecount++;
	}
	StartRecoil();
}

void UPlayerWeaponComponent::StartAiming()
{
	Cast<USpringArmComponent>(owner->GetComponentByClass(USpringArmComponent::StaticClass()))->TargetArmLength = 60.0f;
}

void UPlayerWeaponComponent::StopAiming()
{
	Cast<USpringArmComponent>(owner->GetComponentByClass(USpringArmComponent::StaticClass()))->TargetArmLength = 120.0f;
}

void UPlayerWeaponComponent::StartFire()
{
	Fire();
	isFire = true;
	//startRot = owner->GetController()->GetControlRotation();
	owner->GetWorldTimerManager().SetTimer(fHandle, this, &UPlayerWeaponComponent::Fire, m_firerate, true);
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
		recoilTime += p_deltatime * 0.3f;
		float alpha = easeOutExpo(recoilTime, 0, 1.0f, 1.0f)/m_firerate;
		TickCount += 1;

		float y = FMath::Lerp(0.0f, yawRecoilValue, alpha)/ TickCount;
		float p = FMath::Lerp(0.0f, pitchRecoilValue, alpha)/ TickCount;
		owner->AddControllerYawInput(y);
		owner->AddControllerPitchInput(p);
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(alpha));

		if (alpha >= 1)
		{
			StopRecoil();
			recoilTime = 0;
			TickCount = 1;
		}

	}
}

void UPlayerWeaponComponent::StartRecoil()
{
	StopRcovery();
	bRecoil = true;
	yawRecoilValue = FMath::RandRange(yawRange.X, yawRange.Y);
	pitchRecoilValue = FMath::RandRange(pitchRange.X, pitchRange.Y);
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::SanitizeFloat(yawRecoilValue));
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::SanitizeFloat(pitchRecoilValue));
}

void UPlayerWeaponComponent::StopRecoil()
{
	bRecoil = false;
	/*if (!bRecovery && !isFire)
	{
		StartRecovery();
	}*/
}

void UPlayerWeaponComponent::RecoveryTick(float p_deltatime)
{
	if (bRecovery)
	{
		RecoveryTime += p_deltatime * 2.0f;
		FRotator nowrot = owner->GetController()->GetControlRotation();
		FRotator rc = nowrot; //= FMath::Lerp(nowrot, startRot, RecoveryTime);
		//rc.Yaw = FMath::Lerp(nowrot.Yaw, startRot.Yaw, RecoveryTime)/ TickCount;
		rc.Pitch = FMath::Lerp(nowrot.Pitch, startRot.Pitch, RecoveryTime);
		//rc.Roll = FMath::Lerp(nowrot.Roll, startRot.Roll, RecoveryTime);
		owner->GetController()->SetControlRotation(rc);
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, nowrot.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, startRot.ToString());

		if (nowrot.Pitch == startRot.Pitch)
		{
			StopRcovery();
			RecoveryTime = 0;
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("equal"));
		}
		if (RecoveryTime >= 1.0f)
		{
			StopRcovery();
			RecoveryTime = 0;
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("endtimre"));
		}
	}
}

void UPlayerWeaponComponent::StartRecovery()
{
	bRecovery = true;
}

void UPlayerWeaponComponent::StopRcovery()
{
	bRecovery = false;
}

float UPlayerWeaponComponent::easeOutExpo(float t, float b, float c, float d)
{
	return (t == d) ? b + c : c * (-pow(2, -10 * t / d) + 1) + b;
}

void UPlayerWeaponComponent::SpawnDecal(FHitResult result)
{
	FVector DecalSize(5.0f, 5.0f, 5.0f);
	FRotator DecalRotation = result.Normal.Rotation();
	FVector DecalLocation = result.Location;

	UGameplayStatics::SpawnDecalAtLocation(result.GetActor(), Decal, DecalSize, DecalLocation, DecalRotation, 10.0f);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("decal_spawn"));
}



