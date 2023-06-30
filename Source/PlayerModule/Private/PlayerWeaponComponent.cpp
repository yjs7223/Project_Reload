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
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/DecalComponent.h"
#include "Particles/ParticleSystemComponent.h"

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
	static ConstructorHelpers::FObjectFinder<UParticleSystem> bullet(TEXT("ParticleSystem'/Game/yjs/P_BulletTracer.P_BulletTracer'"));
	if (bullet.Succeeded())
	{
		BulletTracerParticle = bullet.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> decal(TEXT("MaterialInstanceConstant'/Game/yjs/Impact_Decal2.Impact_Decal2'"));
	if (decal.Succeeded())
	{
		Decal = decal.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> fx1(TEXT("NiagaraSystem'/Game/yjs/NS_BulletProjectile.NS_BulletProjectile'"));
	if (fx1.Succeeded())
	{
		shotFXNiagara = fx1.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> fx2(TEXT("NiagaraSystem'/Game/yjs/NS_BulletHit.NS_BulletHit'"));
	if (fx2.Succeeded())
	{
		hitFXNiagara = fx2.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> sound1(TEXT("SoundWave'/Game/ThirdPersonKit/Audio/TPSKitOrginals/weapons/burst_rifle-001.burst_rifle-001'"));
	if (sound1.Succeeded())
	{
		shotsound.Add(sound1.Object);
	}
	static ConstructorHelpers::FObjectFinder<USoundWave> sound2(TEXT("SoundWave'/Game/ThirdPersonKit/Audio/TPSKitOrginals/weapons/burst_rifle-002.burst_rifle-002'"));
	if (sound2.Succeeded())
	{
		shotsound.Add(sound2.Object);
	}
	static ConstructorHelpers::FObjectFinder<USoundWave> sound3(TEXT("SoundWave'/Game/ThirdPersonKit/Audio/TPSKitOrginals/weapons/burst_rifle-003.burst_rifle-003'"));
	if (sound3.Succeeded())
	{
		shotsound.Add(sound3.Object);
	}
	static ConstructorHelpers::FObjectFinder<USoundWave> sound4(TEXT("SoundWave'/Game/ThirdPersonKit/Audio/TPSKitOrginals/weapons/burst_rifle-004.burst_rifle-004'"));
	if (sound4.Succeeded())
	{
		shotsound.Add(sound4.Object);
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
	m_spreadPower = 1.5f;
	yawRange = FVector2D(-0.2f, 0.2f);
	pitchRange = FVector2D(-0.3f, -0.7f);
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
	m_result = FHitResult();
	FCollisionQueryParams param(NAME_None, true, owner);
	FRotator m_rot;
	GameStatic->SpawnEmitterAttached(MuzzleFireParticle, WeaponMesh, FName("MuzzleFlashSocket"));

	//카메라 트레이스
	//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 2.0f);
	if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_Visibility, param))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("camera_hit"));
		//카메라에서 발사한 트레이스 적중
		//DrawDebugPoint(GetWorld(), m_result.Location, 10, FColor::Red, false, 2.f, 0);

		start = WeaponMesh->GetSocketLocation(TEXT("SilencerMuzzleFlashSocket"));
		m_rot = UKismetMathLibrary::FindLookAtRotation(start, m_result.Location);
		end = m_rot.Vector() * 99999;

		//총구 트레이스
		//DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, 2.0f);
		if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_Visibility, param))
		{
			//총구트레이스 적중
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("muzzle_hit"));
			//DrawDebugPoint(GetWorld(), m_result.Location, 10, FColor::Blue, false, 2.f, 0);

			end = m_rot.Vector() * 99999;
			m_rot = UKismetMathLibrary::FindLookAtRotation(start, end);
			SpawnDecal(m_result);
			isHit = true;
		}
		else
		{
			//총구트레이스 비적중
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("muzzle_nonhit"));
			SpawnDecal(m_result);
		}
	}
	else
	{
		//카메라에서 발사한 트레이스 비적중
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("camera_nonhit"));

		//총구 트레이스
		start = WeaponMesh->GetSocketLocation(TEXT("SilencerMuzzleFlashSocket"));
		//DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, 2.0f);
		if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_Visibility, param))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("muzzle_hit"));
			//DrawDebugPoint(GetWorld(), m_result.Location, 10, FColor::Blue, false, 2.f, 0);

			m_rot = UKismetMathLibrary::FindLookAtRotation(start, m_result.Location);
			end = m_rot.Vector() * 99999;
			SpawnDecal(m_result);
			isHit = true;
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("muzzle_nonhit"));
			m_rot = UKismetMathLibrary::FindLookAtRotation(start, end);
		}
	}

	start = WeaponMesh->GetSocketLocation(TEXT("SilencerMuzzleFlashSocket"));
	//shotFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, shotFXNiagara, start, m_rot.GetNormalized());
	hitFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, hitFXNiagara, m_result.Location);
	GameStatic->SpawnEmitterAttached(BulletTracerParticle, WeaponMesh, FName("MuzzleFlashSocket"));
	//shotFXComponent->SetNiagaraVariableVec3("BeamEnd", m_result.Location);
	PlayRandomShotSound();

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

//폐기
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

	UDecalComponent* decal = UGameplayStatics::SpawnDecalAtLocation(result.GetActor(), Decal, DecalSize, DecalLocation, DecalRotation, 10.0f);
	if (decal)
	{
		decal->SetFadeScreenSize(0.0f);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("decal_spawn"));
}

void UPlayerWeaponComponent::PlayRandomShotSound()
{
	int r = FMath::RandRange(0, 3);
	UGameplayStatics::PlaySoundAtLocation(this, shotsound[r], WeaponMesh->GetSocketLocation(TEXT("MuzzleFlashSocket")));
}



