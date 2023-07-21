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
#include "PlayerInputComponent.h"
#include "StatComponent.h"
#include "PlayerWeaponData.h"
#include "MatineeCameraShake.h"
#include "Field/FieldSystemActor.h"


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

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> fx2(TEXT("NiagaraSystem'/Game/Effect/Hit_Effect/Fx_Bullet_Wall_Impact.Fx_Bullet_Wall_Impact'"));
	if (fx2.Succeeded())
	{
		hitFXNiagara = fx2.Object;
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> fx3(TEXT("NiagaraSystem'/Game/yjs/NS_BulletHeadHit.NS_BulletHeadHit'"));
	if (fx3.Succeeded())
	{
		headhitFXNiagara = fx3.Object;
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
	static ConstructorHelpers::FObjectFinder<UBlueprint> fActor(TEXT("Blueprint'/Game/Effect/Destruction/Bomb_Bp.Bomb_Bp'"));
	if (fActor.Succeeded())
	{
		fieldActor = fActor.Object;
	}
	weapontype = EWeaponType::TE_Pistol;

	switch (weapontype)
	{
	case EWeaponType::TE_Pistol:
		WeaponMesh->SetSkeletalMesh(PistolMesh);
		break;
	case EWeaponType::TE_Rifle:
		WeaponMesh->SetSkeletalMesh(RifleMesh);
		break;
	case EWeaponType::TE_Shotgun:
		WeaponMesh->SetSkeletalMesh(ShotgunMesh);
		break;
	default:
		WeaponMesh->SetSkeletalMesh(RifleMesh);
		break;
	}
}

void UPlayerWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	InitData();
	// ...
	//PlayerWeaponData.row
}


// Called every frame
void UPlayerWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	RecoilTick(DeltaTime);
	ReloadTick(DeltaTime);
	RecoveryTick(DeltaTime);
	if (isHit)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("hit"));
	}
}

void UPlayerWeaponComponent::InitData()
{
	owner = GetOwner<APlayerCharacter>();
	if (PlayerWeaponData != nullptr)
	{
		FPlayerweaponStruct* data;
		switch (weapontype)
		{
		case EWeaponType::TE_Pistol:
			data = PlayerWeaponData->FindRow<FPlayerweaponStruct>(FName("Pistol"), FString(""));
			break;
		case EWeaponType::TE_Rifle:
			data = PlayerWeaponData->FindRow<FPlayerweaponStruct>(FName("Rifle"), FString(""));
			break;
		case EWeaponType::TE_Shotgun:
			data = PlayerWeaponData->FindRow<FPlayerweaponStruct>(FName("Shotgun"), FString(""));
			break;
		default:
			data = PlayerWeaponData->FindRow<FPlayerweaponStruct>(FName("Default"), FString(""));
			break;
		}
		
		SetAmmo(data->bullet_Num);

		damage.X = data->max_Damage;
		damage.Y = data->min_Damage;

		H_damage.X = data->max_H_Damage;
		H_damage.Y = data->min_H_Damage;

		m_firecount = 0;
		m_dValue = 0.f;
		recoilTime = 0.0f;
		m_spreadPower = data->spread_Power;
		yawRange = FVector2D(data->min_Horizontal_Recoil, data->max_Horizontal_Recoil);
		pitchRange = FVector2D(data->min_vertical_Recoil, data->max_vertical_Recoil);
		m_firerate = data->fire_Rate;
		TickCount = 1;
		headhit = false;
		reloadCount = 0;
		reloadvalue = 0;
		ammoinfinite = false;
	}
	// ...
}

void UPlayerWeaponComponent::Fire()
{
	if (curAmmo <= 0)
	{
		StopFire();
		return;
	}

	if (!ammoinfinite)
	{
		curAmmo--;
	}
	PlayCameraShake(1.0f);
	FVector start;
	FRotator cameraRotation;
	FVector end;
	owner->Controller->GetPlayerViewPoint(start, cameraRotation);
	float spread = 0;
	if (isAiming)
	{
		spread = m_firecount * m_spreadPower * 0.5;
	}
	else
	{
		spread = m_firecount * m_spreadPower;
	}

	cameraRotation.Yaw += FMath::RandRange(-spread, spread);
	cameraRotation.Pitch += FMath::RandRange(-spread, spread);
	end = start + (cameraRotation.Vector() * 99999);
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("fire"));
	FCollisionQueryParams param(NAME_None, true, owner);
	FRotator m_rot;
	GameStatic->SpawnEmitterAttached(MuzzleFireParticle, WeaponMesh, FName("MuzzleFlashSocket"));

	//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 2.0f);
	if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_Visibility, param))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("camera_hit"));
		//DrawDebugPoint(GetWorld(), m_result.Location, 10, FColor::Red, false, 2.f, 0);

		start = WeaponMesh->GetSocketLocation(TEXT("SilencerMuzzleFlashSocket"));
		m_rot = UKismetMathLibrary::FindLookAtRotation(start, m_result.Location);
		end = m_rot.Vector() * 99999;

		//DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, 2.0f);
		if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_Visibility, param))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("muzzle_hit"));
			//DrawDebugPoint(GetWorld(), m_result.Location, 10, FColor::Blue, false, 2.f, 0);

			end = m_rot.Vector() * 99999;
			m_rot = UKismetMathLibrary::FindLookAtRotation(start, end);
			SpawnDecal(m_result);
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("muzzle_nonhit"));
			SpawnDecal(m_result);
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("camera_nonhit"));

		start = WeaponMesh->GetSocketLocation(TEXT("SilencerMuzzleFlashSocket"));
		//DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, 2.0f);
		if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_Visibility, param))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("muzzle_hit"));
			//DrawDebugPoint(GetWorld(), m_result.Location, 10, FColor::Blue, false, 2.f, 0);

			m_rot = UKismetMathLibrary::FindLookAtRotation(start, m_result.Location);
			end = m_rot.Vector() * 99999;
			SpawnDecal(m_result);
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("muzzle_nonhit"));
			m_rot = UKismetMathLibrary::FindLookAtRotation(start, end);
		}
	}

	if (m_result.GetActor())
	{
		if (m_result.GetActor()->ActorHasTag("Enemy"))
		{
			isHit = true;
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, m_result.GetActor()->GetName());
			//USkeletalMeshComponent* sm = m_result.GetActor()->FindComponentByClass<USkeletalMeshComponent>();
			UStatComponent* s = m_result.GetActor()->FindComponentByClass<UStatComponent>();
			if (s)
			{
				float d = 0;
				if (m_result.BoneName == "head")
				{
					d = FMath::RandRange(H_damage.X, H_damage.Y);

					s->Attacked(d, m_result);

					headhit = true;
					hitFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, headhitFXNiagara, m_result.Location);
				}
				else
				{
					d = FMath::RandRange(damage.X, damage.Y);
					s->Attacked(d, m_result);
					hitFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, hitFXNiagara, m_result.Location);
				}
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::SanitizeFloat(d));
			}
		}
		else
		{
			hitFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, hitFXNiagara, m_result.Location);
			FActorSpawnParameters spawnparam;
			spawnparam.Owner = owner;
			TSubclassOf<UObject> fieldbp = fieldActor->GeneratedClass;
			GetWorld()->SpawnActor<AActor>(fieldbp, m_result.Location, FRotator::ZeroRotator, spawnparam);

		}
	}
	else
	{
		hitFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, hitFXNiagara, m_result.Location);
	}

	if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_GameTraceChannel2, param))
	{
		if (m_result.GetActor()->ActorHasTag("Enemy"))
		{
			UStatComponent* s = m_result.GetActor()->FindComponentByClass<UStatComponent>();

			if (s)
			{
				float d = FMath::RandRange(H_damage.X, H_damage.Y);
				s->Attacked(d);
			}
		}
	}

	start = WeaponMesh->GetSocketLocation(TEXT("BulletTracerStart"));
	GameStatic->SpawnEmitterAtLocation(GetWorld(), BulletTracerParticle, start, m_rot);
	//shotFXComponent->SetNiagaraVariableVec3("BeamEnd", m_result.Location);
	PlayRandomShotSound();

	if(m_firecount < 10)
	{
		m_firecount += 1;
	}
	StartRecoil();
}

void UPlayerWeaponComponent::StartAiming()
{
	Cast<USpringArmComponent>(owner->GetComponentByClass(USpringArmComponent::StaticClass()))->TargetArmLength = 60.0f;
	FVector start;
	FRotator cameraRotation;
	FVector end;
	isAiming = true;
	owner->Controller->GetPlayerViewPoint(start, cameraRotation);
}

void UPlayerWeaponComponent::StopAiming()
{
	isAiming = false;
	Cast<USpringArmComponent>(owner->GetComponentByClass(USpringArmComponent::StaticClass()))->TargetArmLength = 120.0f;
}

void UPlayerWeaponComponent::StartFire()
{
	if(curAmmo <= 0)
	{ 
		return;
	}

	StopRcovery();
	Fire();
	isFire = true;
	startRot = owner->GetController()->GetControlRotation();
	if (weapontype == EWeaponType::TE_Rifle)
	{
		owner->GetWorldTimerManager().SetTimer(fHandle, this, &UPlayerWeaponComponent::Fire, m_firerate, true);
	}
}

void UPlayerWeaponComponent::StopFire()
{
	if (isFire)
	{
		if (weapontype == EWeaponType::TE_Rifle)
		{
			owner->GetWorldTimerManager().ClearTimer(fHandle);
		}
		owner->FindComponentByClass<UPlayerInputComponent>()->getInput()->IsFire = false;
		isFire = false;
		StartRecovery();
	}
}

void UPlayerWeaponComponent::StartReload()
{
	//ReloadAmmo();

	switch (weapontype)
	{
	case EWeaponType::TE_Pistol:
		if (curAmmo >= 10)
		{
			isReload = false;
			return;
		}

		reloadvalue = 10;

		curAmmo = 0;
		break;
	case EWeaponType::TE_Rifle:
		if (maxAmmo <= 0)
		{
			isReload = false;
			return;
		}

		if (curAmmo >= 30)
		{
			isReload = false;
			return;
		}

		reloadvalue = 30 - curAmmo;
		if (maxAmmo < reloadvalue)
		{
			reloadvalue = maxAmmo;
			maxAmmo = 0;
		}
		else
		{
			maxAmmo -= reloadvalue;
		}

		curAmmo = 0;
		break;
	case EWeaponType::TE_Shotgun:

		break;
	default:
		if (maxAmmo <= 0)
		{
			isReload = false;
			return;
		}

		if (curAmmo >= 30)
		{
			isReload = false;
			return;
		}

		reloadvalue = 30 - curAmmo;
		if (maxAmmo < reloadvalue)
		{
			reloadvalue = maxAmmo;
			maxAmmo = 0;
		}
		else
		{
			maxAmmo -= reloadvalue;
		}

		curAmmo = 0;
		break;
	}

	isReload = true;
}

void UPlayerWeaponComponent::StopReload()
{
	reloadvalue = 0;
	isReload = false;
}

void UPlayerWeaponComponent::ReloadTick(float Deltatime)
{
	if (isReload)
	{
		reloadCount += Deltatime * 20;
		if (reloadCount >= 1)
		{
			curAmmo++;
			reloadCount = 0;
		}

		switch (weapontype)
		{
		case EWeaponType::TE_Pistol:
			if (curAmmo == 10)
			{
				StopReload();
			}
			break;
		case EWeaponType::TE_Rifle:
			if (maxAmmo == 0)
			{
				if (curAmmo == reloadvalue)
				{
					StopReload();
				}
			}

			if (curAmmo == 30)
			{
				StopReload();
			}
			break;
		case EWeaponType::TE_Shotgun:
			break;
		default:
			if (maxAmmo == 0)
			{
				if (curAmmo == reloadvalue)
				{
					StopReload();
				}
			}

			if (curAmmo == 30)
			{
				StopReload();
			}
			break;
		}
	}
}

void UPlayerWeaponComponent::RecoilTick(float p_deltatime)
{
	if (bRecoil)
	{
		recoilTime += p_deltatime * 0.3f;
		float alpha = easeOutExpo(recoilTime, 0, 1.0f, 1.0f)/m_firerate;
		TickCount += 1;
		recoilAlpha = alpha;
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
			recoilAlpha = 0;
		}

	}
}

void UPlayerWeaponComponent::StartRecoil()
{
	bRecovery = false;
	bRecoil = true;
	yawRecoilValue = FMath::RandRange(yawRange.X, yawRange.Y);
	pitchRecoilValue = FMath::RandRange(pitchRange.X, pitchRange.Y);
	//yawRecoveryValue += yawRecoilValue;
	//pitchRecoveryValue += pitchRecoilValue;
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("recoil start"));
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::SanitizeFloat(pitchRecoilValue));
}

void UPlayerWeaponComponent::StopRecoil()
{
	bRecoil = false;
}

//���
void UPlayerWeaponComponent::RecoveryTick(float p_deltatime)
{
	if (bRecovery)
	{
		RecoveryTime += p_deltatime * 2.0f;
		TickCount += 2;
		if (FMath::Abs(yawRecoveryValue) > 5.0f)
		{
			TickCount += 1000;
		}
		if (pitchRecoveryValue < -20.0f)
		{
			TickCount += 10;
		}
		float y = FMath::Lerp(0.0f, yawRecoveryValue, RecoveryTime) / TickCount;
		float p = FMath::Lerp(0.0f, -pitchRecoveryValue, RecoveryTime) / TickCount;
		owner->AddControllerYawInput(y);
		owner->AddControllerPitchInput(p);
		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::SanitizeFloat(yawRecoveryValue));
		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::SanitizeFloat(pitchRecoveryValue));

		if (RecoveryTime >= 0.8f)
		{
			StopRcovery();
		}
	}
}

void UPlayerWeaponComponent::StartRecovery()
{
	bRecovery = true;
	StopRecoil();
	FRotator nowrot = owner->GetController()->GetControlRotation();
	//recoveryRot = nowrot - startRot;
	if (startRot.Yaw <= 90)
	{
		startRot.Yaw += 360;
	}
	if (nowrot.Yaw <= 90)
	{
		nowrot.Yaw += 360;
	}
	if (startRot.Pitch <= 90)
	{
		startRot.Pitch += 360;
	}
	if (nowrot.Pitch <= 90)
	{
		nowrot.Pitch += 360;
	}
	yawRecoveryValue = startRot.Yaw - nowrot.Yaw;
	pitchRecoveryValue = startRot.Pitch - nowrot.Pitch;
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::SanitizeFloat(yawRecoveryValue));
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::SanitizeFloat(pitchRecoveryValue));
	//StopRcovery();

}

void UPlayerWeaponComponent::StopRcovery()
{
	bRecovery = false;
	yawRecoveryValue = 0;
	pitchRecoveryValue = 0;
	RecoveryTime = 0;
	TickCount = 0;
	//startRot = FRotator::ZeroRotator;
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("end recovery"));
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

void UPlayerWeaponComponent::PlayCameraShake(float scale)
{
	if (fireShake != nullptr)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(fireShake, scale);
	}
}



