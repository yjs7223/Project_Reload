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
#include "PlayerInputComponent.h"
#include "Field/FieldSystemActor.h"
#include "WeaponDataAsset.h"
#include "HitImapactDataAsset.h"
#include "Perception/AISense_Hearing.h"
#include "Components/WidgetComponent.h"
#include "PlayerMoveComponent.h"
#include "CoverComponent.h"


UPlayerWeaponComponent::UPlayerWeaponComponent()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/yjs/DT_PlayerWeaponData.DT_PlayerWeaponData'"));
	if (DataTable.Succeeded())
	{
		PlayerWeaponData = DataTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataAsset> rifle_da(TEXT("WeaponDataAsset'/Game/yjs/DA_Rifle.DA_Rifle'"));
	if (rifle_da.Succeeded())
	{

		RifleDataAssets = Cast<UWeaponDataAsset>(rifle_da.Object);
	}

	static ConstructorHelpers::FObjectFinder<UDataAsset> pistol_da(TEXT("WeaponDataAsset'/Game/yjs/DA_Pistol.DA_Pistol'"));
	if (pistol_da.Succeeded())
	{
		PistolDataAssets = Cast<UWeaponDataAsset>(pistol_da.Object);
	}

	static ConstructorHelpers::FObjectFinder<UDataAsset> hitimpact(TEXT("HitImapactDataAsset'/Game/yjs/DA_HItImapct.DA_HItImapct'"));
	if (hitimpact.Succeeded())
	{
		HitImpactDataAsset = Cast<UHitImapactDataAsset>(hitimpact.Object);
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> fActor(TEXT("Blueprint'/Game/Effect/Destruction/Bomb_Bp.Bomb_Bp'"));
	if (fActor.Succeeded())
	{
		fieldActor = fActor.Object;
	}
	weapontype = EWeaponType::TE_Rifle;
	if (RifleDataAssets)
	{
		if (RifleDataAssets->WeaponSkeletalMesh)
		{
			WeaponMesh->SetSkeletalMesh(RifleDataAssets->WeaponSkeletalMesh);
		}

		if (RifleDataAssets->weaponAnim)
		{
			WeaponMesh->SetAnimInstanceClass(RifleDataAssets->weaponAnim);
		}

		FVector location = FVector::ZeroVector;
		location = FVector(1.619504, 0.306273, 2.024439) * FVector(-1.0, -1.0, 1.0);
		WeaponMesh->SetRelativeLocation(location);
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

	if (isAiming)
	{
		if (!owner->FindComponentByClass<UCoverComponent>()->IsCover())
		{
			owner->FindComponentByClass<UPlayerMoveComponent>()->Turn();
		}
	}
}

void UPlayerWeaponComponent::InitData()
{
	owner = GetOwner<APlayerCharacter>();
	if (PlayerWeaponData != nullptr)
	{
		FPlayerweaponStruct* dataTable;
		UWeaponDataAsset* WeapondataAsset = nullptr;
		switch (weapontype)
		{
		case EWeaponType::TE_Pistol:
			dataTable = PlayerWeaponData->FindRow<FPlayerweaponStruct>(FName("Pistol"), FString(""));
			WeapondataAsset = PistolDataAssets;
			break;
		case EWeaponType::TE_Rifle:
			dataTable = PlayerWeaponData->FindRow<FPlayerweaponStruct>(FName("Rifle"), FString(""));
			WeapondataAsset = RifleDataAssets;
			break;
		case EWeaponType::TE_Shotgun:
			dataTable = PlayerWeaponData->FindRow<FPlayerweaponStruct>(FName("Shotgun"), FString(""));
			break;
		default:
			dataTable = PlayerWeaponData->FindRow<FPlayerweaponStruct>(FName("Default"), FString(""));
			WeapondataAsset = RifleDataAssets;
			break;
		}
		if (WeapondataAsset)
		{
			//WeaponMesh.set
			WeaponMeshSetting(WeapondataAsset);
			MuzzleFireParticle = WeapondataAsset->MuzzleFireParticle;
			BulletTracerParticle = WeapondataAsset->BulletTracerParticle;
			shotFXNiagara = WeapondataAsset->BulletTrailFXNiagara;

			ShotSounds = WeapondataAsset->ShotSounds;

			Decal = WeapondataAsset->Decals[0];
		}
		
		SetAmmo(dataTable->bullet_Num);

		damage.X = dataTable->max_Damage;
		damage.Y = dataTable->min_Damage;

		H_damage.X = dataTable->max_H_Damage;
		H_damage.Y = dataTable->min_H_Damage;

		Deviation = dataTable->Deviation;
		MaxRange = dataTable->MaxRange;

		m_firecount = 0;
		m_dValue = 0.f;
		recoilTime = 0.0f;
		m_spreadPower = dataTable->spread_Power;
		yawRange = FVector2D(dataTable->min_Horizontal_Recoil, dataTable->max_Horizontal_Recoil);
		pitchRange = FVector2D(dataTable->min_vertical_Recoil, dataTable->max_vertical_Recoil);
		m_firerate = dataTable->fire_Rate;
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
	if (!m_CanShooting) return;
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

	//CameraHit
	//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 2.0f);
	if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_Visibility, param))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("camera_hit"));
		//DrawDebugPoint(GetWorld(), m_result.Location, 10, FColor::Red, false, 2.f, 0);

		start = WeaponMesh->GetSocketLocation(TEXT("SilencerMuzzleFlashSocket"));
		m_rot = UKismetMathLibrary::FindLookAtRotation(start, m_result.Location);
		end = m_rot.Vector() * 99999;

		//WeaponHit
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

		//WeaponHit
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
			//NonHit
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("muzzle_nonhit"));
			m_rot = UKismetMathLibrary::FindLookAtRotation(start, end);
		}
	}
	//UGameplayStatics::

	
	if (CheckActorTag(m_result.GetActor(), TEXT("Enemy")))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, m_result.GetActor()->GetName());
		UStatComponent* MyStat = m_result.GetActor()->FindComponentByClass<UStatComponent>();
		if (MyStat)
		{
			if (!MyStat->isDie)
			{
				isHit = true;
				float damageVlaue = 0;
				if (m_result.BoneName == "head")
				{
					damageVlaue = CalcDamage(m_result, H_damage);
					MyStat->Attacked(damageVlaue, m_result);

					headhit = true;
				}
				else
				{
					damageVlaue = CalcDamage(m_result, damage);
					MyStat->Attacked(damageVlaue, m_result);
				}
				owner->CreateDamageWidget(damageVlaue, m_result);
				OnChangedCrossHairDieDelegate.ExecuteIfBound();
			}
		}
	}
	else
	{
		FActorSpawnParameters spawnparam;
		spawnparam.Owner = owner;
		TSubclassOf<UObject> fieldbp = fieldActor->GeneratedClass;
		GetWorld()->SpawnActor<AActor>(fieldbp, m_result.Location, FRotator::ZeroRotator, spawnparam);

	}
		
	OnChangedCrossHairHitDelegate.ExecuteIfBound();

	start = WeaponMesh->GetSocketLocation(TEXT("MuzzleFlashSocket"));
	//UGameplayStatics::SpawnEmitterAtLocation()
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletTracerParticle, start, m_rot);
	//shotFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), shotFXNiagara, start);
	//shotFXComponent->SetNiagaraVariableVec3("Beam_end", m_result.Location);

	SpawnImpactEffect(m_result);
	PlayRandomShotSound();

	if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_GameTraceChannel3, param))
	{
		if (CheckActorTag(m_result.GetActor(), TEXT("Enemy")))
		{
			UStatComponent* MyStat = m_result.GetActor()->FindComponentByClass<UStatComponent>();

			if (MyStat)
			{
				float damageVlaue = FMath::RandRange(H_damage.X, H_damage.Y);
				MyStat->Attacked(damageVlaue);
			}

		}

	}

	if(m_firecount < 10)
	{
		m_firecount += 1;
	}
	OnChangedCrossHairAmmoDelegate.ExecuteIfBound();
	OnChangedAmmoUIDelegate.ExecuteIfBound();
	StartRecoil();

	if (!owner->FindComponentByClass<UCoverComponent>()->IsCover())
	{
		owner->FindComponentByClass<UPlayerMoveComponent>()->Turn();
	}
}

void UPlayerWeaponComponent::StartAiming()
{
	FVector start;
	FRotator cameraRotation;
	FVector end;
	isAiming = true;
	owner->Controller->GetPlayerViewPoint(start, cameraRotation);
	owner->HPWidgetComponent->AttachToComponent(owner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Aiming_HP_Socket"));
	owner->GetWorldTimerManager().SetTimer(AimingTimer, this, &UPlayerWeaponComponent::Threaten, 0.3, true, 0.0f);

	OnVisibleCrossHairUIDelegate.ExecuteIfBound();
	OnVisibleAmmoUIDelegate.ExecuteIfBound();
}

void UPlayerWeaponComponent::StopAiming()
{
	isAiming = false;
	owner->HPWidgetComponent->AttachToComponent(owner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("HP_Widget_Socket"));
	owner->GetWorldTimerManager().ClearTimer(AimingTimer);
	OnVisibleCrossHairUIDelegate.ExecuteIfBound();
	OnVisibleAmmoUIDelegate.ExecuteIfBound();
}

void UPlayerWeaponComponent::StartFire()
{
	if(curAmmo <= 0)
	{ 
		owner->FindComponentByClass<UPlayerInputComponent>()->StartReload();
		return;
	}

	StopRcovery();
	Fire();
	isFire = true;

	OnVisibleCrossHairUIDelegate.ExecuteIfBound();
	OnVisibleAmmoUIDelegate.ExecuteIfBound();

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
		
		OnVisibleCrossHairUIDelegate.ExecuteIfBound();
		OnVisibleAmmoUIDelegate.ExecuteIfBound();
		StartRecovery();
	}
}

void UPlayerWeaponComponent::StartReload()
{
	//ReloadAmmo();
	StopFire();
	if (isReload)
	{
		return;
	}

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
		if (holdAmmo <= 0)
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
		if (holdAmmo < reloadvalue)
		{
			reloadvalue = holdAmmo;
			holdAmmo = 0;
		}
		else
		{
			holdAmmo -= reloadvalue;
		}

		curAmmo = 0;
		break;
	case EWeaponType::TE_Shotgun:

		break;
	default:
		if (holdAmmo <= 0)
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
		if (holdAmmo < reloadvalue)
		{
			reloadvalue = holdAmmo;
			holdAmmo = 0;
		}
		else
		{
			holdAmmo -= reloadvalue;
		}

		curAmmo = 0;
		break;
	}
	OnPlayReloadUIDelegate.ExecuteIfBound();
	isReload = true;
}

void UPlayerWeaponComponent::StopReload()
{
	reloadvalue = 0;
	isReload = false;
}

void UPlayerWeaponComponent::WeaponMeshSetting(UWeaponDataAsset* WeapondataAsset)
{
	if (WeapondataAsset)
	{
		FVector location = FVector::ZeroVector;

		if (WeapondataAsset->WeaponSkeletalMesh)
		{
			WeaponMesh->SetSkeletalMesh(WeapondataAsset->WeaponSkeletalMesh);
		}

		if (WeapondataAsset->weaponAnim) 
		{
			WeaponMesh->SetAnimInstanceClass(RifleDataAssets->weaponAnim);
		}

		switch (weapontype)
		{
		case EWeaponType::TE_Pistol:
			break;
		case EWeaponType::TE_Rifle:
			location = FVector(1.619504, 0.306273, 2.024439) * FVector(-1.0, -1.0, 1.0);
			break;
		case EWeaponType::TE_Shotgun:
			break;
		default:
			break;
		}
		WeaponMesh->SetRelativeLocation(location);
	}
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
			OnChangedCrossHairAmmoDelegate.ExecuteIfBound();
			OnChangedAmmoUIDelegate.ExecuteIfBound();
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
			if (holdAmmo == 0)
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
			if (holdAmmo == 0)
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
	if (m_firecount == 1)
	{
		yawRecoilValue = FMath::RandRange(yawRange.X, yawRange.Y);
		pitchRecoilValue = FMath::RandRange(pitchRange.X * 2.0f, pitchRange.Y * 1.5f);
	}
	else
	{
		yawRecoilValue = FMath::RandRange(yawRange.X, yawRange.Y);
		pitchRecoilValue = FMath::RandRange(pitchRange.X, pitchRange.Y);
	}
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
	if (CheckActorTag(result.GetActor(), TEXT("Water")))
	{
		return;
	}

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
	UGameplayStatics::PlaySoundAtLocation(this, ShotSounds[r], GetOwner()->GetActorLocation());

	UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetOwner()->GetActorLocation(), 1.0f, GetOwner(), 0.0f, FName(TEXT("Shooting")));
}

void UPlayerWeaponComponent::PlayCameraShake(float scale)
{
	if (fireShake != nullptr)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(fireShake, scale);
	}
}

void UPlayerWeaponComponent::SpawnImpactEffect(FHitResult result)
{
	if (HitImpactDataAsset)
	{

		if (CheckActorTag(result.GetActor(), TEXT("Enemy")))
		{
			if (CheckActorTag(result.GetActor(), TEXT("Robot")))
			{
				hitFXNiagara = HitImpactDataAsset->RobotHitFXNiagara;
			}
			else if (CheckActorTag(result.GetActor(), TEXT("Human")))
			{
				hitFXNiagara = HitImpactDataAsset->HumanHitFXNiagara;
			}
			else
			{
				hitFXNiagara = HitImpactDataAsset->RobotHitFXNiagara;
			}
		}
		else
		{
			if (CheckActorTag(result.GetActor(), TEXT("Metal")))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Metal"));
				hitFXNiagara = HitImpactDataAsset->MetalHitFXNiagara;
			}
			else if (CheckActorTag(result.GetActor(), TEXT("Rock")))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Rock"));
				hitFXNiagara = HitImpactDataAsset->RockHitFXNiagara;
			}
			else if (CheckActorTag(result.GetActor(), TEXT("Mud")))
			{
				hitFXNiagara = HitImpactDataAsset->MudHitFXNiagara;
			}
			else if (CheckActorTag(result.GetActor(), TEXT("Glass")))
			{
				hitFXNiagara = HitImpactDataAsset->GlassHitFXNiagara;
			}
			else if (CheckActorTag(result.GetActor(), TEXT("Water")))
			{
				hitFXNiagara = HitImpactDataAsset->WaterHitFXNiagara;
			}
			else
			{
				hitFXNiagara = HitImpactDataAsset->MetalHitFXNiagara;
			}
		}
	}

	FRotator m_rot = UKismetMathLibrary::FindLookAtRotation(result.Location, GetOwner()->GetActorLocation());
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, m_rot.ToString());
	m_rot.Pitch -= 90.0f;
	hitFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), hitFXNiagara, result.Location, m_rot);

	//hitFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), hitFXNiagara, result.Location);//, m_rot);
}

void UPlayerWeaponComponent::Threaten()
{
	FVector start;
	FRotator cameraRotation;
	FVector end;
	owner->Controller->GetPlayerViewPoint(start, cameraRotation);
	end = start + (cameraRotation.Vector() * 99999);
	FCollisionQueryParams param(NAME_None, true, owner);
	FHitResult result;
	//GetWorld()->SweepSingleByChannel(result, start, end, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(50.0f))
	//DrawDebugSphere(GetWorld(), start, 50.0f, 50.0f, FColor::Red, true);
	if (GetWorld()->SweepSingleByChannel(result, start, end, FQuat::Identity, ECC_GameTraceChannel3, FCollisionShape::MakeSphere(50.0f)))
	{
		//DrawDebugSphere(GetWorld(), result.Location, 50.0f, 50.0f, FColor::Red, true);
		if (result.GetActor())
		{
			if (result.GetActor()->Tags.Num() > 0)
			{
				if (result.GetActor()->ActorHasTag("Enemy"))
				{
					UStatComponent* stat = result.GetActor()->FindComponentByClass<UStatComponent>();
					if (stat)
					{
						stat->isThreat = true;
					}
				}
			}
		}
	}
}

float UPlayerWeaponComponent::CalcDamage(FHitResult result, FVector2D p_damage)
{
	if (MaxRange > 0)
	{
		FVector range = result.Location - owner->GetActorLocation();
		float alpha = range.Length() / MaxRange;
		alpha = FMath::Clamp(alpha, 0.0f, 1.0f);
		float m_dmg = FMath::Lerp(p_damage.X, p_damage.Y, alpha);
		m_dmg += FMath::RandRange(-Deviation, Deviation);
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(m_dmg));
		return m_dmg;
	}
	return 0.0f;
}

bool UPlayerWeaponComponent::CheckActorTag(AActor* actor, FName tag)
{
	if (actor)
	{
		if (actor->Tags.Num() > 0)
		{
			if (actor->ActorHasTag(tag))
			{
				return true;
			}
		}
	}

	return false;
}




