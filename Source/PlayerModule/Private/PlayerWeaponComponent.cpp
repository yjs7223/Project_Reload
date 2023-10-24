// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeaponComponent.h"
#include "PlayerCharacter.h"
#include "Curves/CurveVector.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"
#include "Camera/CameraComponent.h"
#include "BaseInputComponent.h"
#include "Particles/ParticleSystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
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
#include "HitImapactDataAsset.h"
#include "Perception/AISense_Hearing.h"
#include "PlayerMoveComponent.h"
#include "CoverComponent.h"
#include "CharacterSoundDataAsset.h"
#include "Sound/SoundCue.h"
#include "Bullet.h"
#include "EmptyShellSpawnable.h"
#include "PlayerWeaponDataAsset.h"
#include "CharacterSoundDataAsset.h"

UPlayerWeaponComponent::UPlayerWeaponComponent()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/yjs/DT_PlayerWeaponData.DT_PlayerWeaponData'"));
	if (DataTable.Succeeded())
	{
		PlayerWeaponDataTable = DataTable.Object;
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
	
}

void UPlayerWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UStatComponent* statComp = GetOwner()->FindComponentByClass<UStatComponent>())
	{
		statComp->diePlay.__Internal_AddDynamic(this, &UPlayerWeaponComponent::InitData, FName("InitData"));
	}

	InitData();
	
	// ...
	//PlayerWeaponData.row
}

void UPlayerWeaponComponent::BeginDestroy()
{
	OnChangedCrossHairAmmoDelegate.Unbind();
	OnChangedCrossHairHitDelegate.Unbind();
	OnChangedCrossHairDieDelegate.Unbind();
	OnVisibleCrossHairUIDelegate.Unbind();
	OnVisibleAmmoUIDelegate.Clear();
	OnChangedAmmoUIDelegate.Clear();
	OnPlayReloadUIDelegate.Unbind();
	OnSpawnDamageUIDelegate.Unbind();

	Super::BeginDestroy();
}


// Called every frame
void UPlayerWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	RecoilTick(DeltaTime);
	ReloadTick(DeltaTime);
	RecoveryTick(DeltaTime);
	CalculateBlockingTick(DeltaTime);

	if (bAiming)
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
	if (PlayerWeaponDataTable != nullptr)
	{
		FPlayerWeaponStruct* dataTable;
		switch (weapontype)
		{
		case EWeaponType::Pistol:
			dataTable = PlayerWeaponDataTable->FindRow<FPlayerWeaponStruct>(FName("Pistol"), FString(""));
			maxAmmo = dataTable->MaxAmmo_num;
			PlayerWeaponDataAsset = LoadObject<UPlayerWeaponDataAsset>(NULL, TEXT("PlayerWeaponDataAsset'/Game/yjs/DA_PistolAsset.DA_PistolAsset'"));
			break;
		case EWeaponType::Rifle:
			dataTable = PlayerWeaponDataTable->FindRow<FPlayerWeaponStruct>(FName("Rifle"), FString(""));
			maxAmmo = dataTable->MaxAmmo_num;
			PlayerWeaponDataAsset = LoadObject<UPlayerWeaponDataAsset>(NULL, TEXT("PlayerWeaponDataAsset'/Game/yjs/DA_RifleAsset.DA_RifleAsset'"));
			break;
		case EWeaponType::Heavy:
			dataTable = PlayerWeaponDataTable->FindRow<FPlayerWeaponStruct>(FName("Heavy"), FString(""));
			maxAmmo = dataTable->MaxAmmo_num;
			PlayerWeaponDataAsset = LoadObject<UPlayerWeaponDataAsset>(NULL, TEXT("PlayerWeaponDataAsset'/Game/yjs/DA_HeavyAsset.DA_HeavyAsset'"));
			break;
		case EWeaponType::Shotgun:
			dataTable = PlayerWeaponDataTable->FindRow<FPlayerWeaponStruct>(FName("Shotgun"), FString(""));
			break;
		default:
			dataTable = PlayerWeaponDataTable->FindRow<FPlayerWeaponStruct>(FName("Default"), FString(""));
			maxAmmo = 30;
			PlayerWeaponDataAsset = LoadObject<UPlayerWeaponDataAsset>(NULL, TEXT("PlayerWeaponDataAsset'/Game/yjs/DA_RifleAsset.DA_RifleAsset'"));
			break;
		}
		if (PlayerWeaponDataAsset)
		{
			//WeaponMesh.set
			WeaponMeshSetting(PlayerWeaponDataAsset);
			for (auto& item : PlayerWeaponDataAsset->Attachments)
			{
				UStaticMeshComponent* meshcomp = NewObject<UStaticMeshComponent>(owner);

				meshcomp->SetupAttachment(WeaponMesh, item.Key);
				meshcomp->SetStaticMesh(item.Value);
				meshcomp->RegisterComponentWithWorld(owner->GetWorld());
			}
			/*MuzzleFireParticle = WeaponDataAsset->MuzzleFireParticle;
			BulletTracerParticle = WeaponDataAsset->BulletTracerParticle;
			shotFXNiagara = WeaponDataAsset->BulletTrailFXNiagara;*/

			//ShotSounds = WeaponDataAsset->ShotSounds;

			//Decal = WeaponDataAsset->Decals[0];
		}
		
		maxAmmo = dataTable->MaxAmmo_num;
		curAmmo = maxAmmo;
		holdAmmo = dataTable->HoldAmmo_num;

		damage.X = dataTable->Max_Damage;
		damage.Y = dataTable->Min_Damage;

		head_mag = dataTable->Head_Magnification;

		Deviation = dataTable->Deviation;
		MaxRange = dataTable->Max_Range;

		m_firecount = 0;
		recoilTime = 0.0f;
		m_spreadPower = dataTable->spread_Power;
		yawRange = FVector2D(dataTable->min_Horizontal_Recoil, dataTable->max_Horizontal_Recoil);
		pitchRange = FVector2D(dataTable->min_vertical_Recoil, dataTable->max_vertical_Recoil);
		m_firerate = dataTable->Fire_Rate;
		MaxPitchRecoilValue = dataTable->MaxPitchRecoilValue;
		AimingRecoilValue = dataTable->AimingRecoilValue;
		TickCount = 1;
		headhit = false;
		reloadCount = 0;
		reloadvalue = 0;
		ammoinfinite = false;
		m_WeaponDistance = dataTable->WeaponDistance;


		OnChangedAmmoUIDelegate.Broadcast();
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
	Super::Fire();

	if (bReload)
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
	if (bAiming)
	{
		spread = m_firecount * m_spreadPower * AimingRecoilValue;
	}
	else
	{
		spread = m_firecount * m_spreadPower;
	}

	cameraRotation.Yaw += FMath::RandRange(-spread, spread);
	cameraRotation.Pitch += FMath::RandRange(-spread, spread);
	end = start + (cameraRotation.Vector() * 15000.0);
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("fire"));
	FCollisionQueryParams param(NAME_None, true, owner);
	FRotator m_rot;
	UGameplayStatics::SpawnEmitterAttached(PlayerWeaponDataAsset->MuzzleFireParticle, WeaponMesh, FName("MuzzleFlashSocket"));
	
	FActorSpawnParameters spawnparam;
	spawnparam.Owner = owner;
	//CameraHit
	//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 112.0f);
	if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_GameTraceChannel6, param))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, m_result.GetComponent()->GetName());
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("camera_hit"));
		//DrawDebugPoint(GetWorld(), m_result.Location, 10, FColor::Red, false, 2.f, 0);

		UCameraComponent* camera = GetOwner()->FindComponentByClass<UCameraComponent>();

		FVector mindis = m_result.Location - camera->GetComponentLocation();

		float dis = (camera->GetComponentLocation() - GetOwner()->GetActorLocation()).Length(); //200 100

		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(mindis.Length()));
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::SanitizeFloat(dis));
		if (mindis.Length() <= dis)
		{
			param.AddIgnoredActor(m_result.GetActor());
			if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_GameTraceChannel6, param))
			{
				DrawDebugPoint(GetWorld(), m_result.Location, 10, FColor::Red, false, 5.f, 0);
				start = WeaponMesh->GetSocketLocation(TEXT("MuzzleFlashSocket"));
				m_rot = UKismetMathLibrary::FindLookAtRotation(start, m_result.Location);
				end = m_rot.Vector() * 1000000.0f;
			}
		}
		else
		{
			start = WeaponMesh->GetSocketLocation(TEXT("MuzzleFlashSocket"));
			m_rot = UKismetMathLibrary::FindLookAtRotation(start, m_result.Location);
			end = m_rot.Vector() * 1000000.0f;
		}

	}
	else
	{
		m_rot = UKismetMathLibrary::FindLookAtRotation(start, end);
		
	}

	
	if (CheckActorTag(m_result.GetActor(), TEXT("Enemy")))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, m_result.GetActor()->GetName());
		UStatComponent* MyStat = m_result.GetActor()->FindComponentByClass<UStatComponent>();
		if (MyStat)
		{
			if (!MyStat->bDie)
			{
				bHit = true;
				float damageVlaue = 0;
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, m_result.BoneName.ToString());
				if (m_result.BoneName == "head")
				{
					damageVlaue = CalcDamage(m_result, damage) * head_mag;
					MyStat->Attacked(damageVlaue, owner);
					headhit = true;
				}
				else
				{
					damageVlaue = CalcDamage(m_result, damage);
					MyStat->Attacked(damageVlaue, owner);
				}

				if (bHit)
				{
					OnSpawnDamageUIDelegate.ExecuteIfBound(damageVlaue, m_result);
					MyStat->hitNormal = m_result.TraceEnd - m_result.TraceStart;
					OnChangedCrossHairDieDelegate.ExecuteIfBound();
				}
			}
		}
	}
		
	OnChangedCrossHairHitDelegate.ExecuteIfBound();

	start = WeaponMesh->GetSocketLocation(TEXT("MuzzleFlashSocket"));
	ABullet* bullet = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), start, m_rot, spawnparam);
	if (bullet)
	{
		bullet->SpawnBulletFx(PlayerWeaponDataAsset->BulletTrailFXNiagara, m_rot.Vector(), owner, PlayerWeaponDataAsset->Bullet_Passby_Sound);
		bullet->OnBulletHitDelegate.AddUObject(this, &UPlayerWeaponComponent::SpawnImpactEffect);
		bullet->OnBulletHitDelegate.AddUObject(this, &UPlayerWeaponComponent::SpawnDecal);
		bullet->OnBulletHitDelegate.AddUObject(this, &UPlayerWeaponComponent::SpawnField);
	}
	//SpawnImpactEffect(m_result);
	PlayRandomShotSound();

	if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_GameTraceChannel3, param))
	{
		if (CheckActorTag(m_result.GetActor(), TEXT("Enemy")))
		{
			UStatComponent* MyStat = m_result.GetActor()->FindComponentByClass<UStatComponent>();

			if (MyStat)
			{
				float damageVlaue = FMath::RandRange(damage.X, damage.Y);
				MyStat->IndirectAttacked(damageVlaue);
			}

		}

	}

	if(m_firecount < 10)
	{
		m_firecount += 1;
	}


	OnChangedCrossHairAmmoDelegate.ExecuteIfBound();
	OnChangedAmmoUIDelegate.Broadcast();
	StartRecoil();
	UAnimInstance* animinstatce = WeaponMesh->GetAnimInstance();
	if (animinstatce->GetClass()->ImplementsInterface(UEmptyShellSpawnable::StaticClass())) {
		IEmptyShellSpawnable::Execute_EmptyShellSpawn((animinstatce));
	}
	//Cast<UWeaponAnimInstance>(owner->GetMesh()->GetAnimInstance()).
	//PlayShootingAnimation
	if (!owner->FindComponentByClass<UCoverComponent>()->IsCover())
	{
		owner->FindComponentByClass<UPlayerMoveComponent>()->Turn();
	}
}

void UPlayerWeaponComponent::StartAiming()
{
	Super::StartAiming();

	FVector start;
	FRotator cameraRotation;
	FVector end;

	owner->Controller->GetPlayerViewPoint(start, cameraRotation);
	//owner->HPWidgetComponent->AttachToComponent(owner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Aiming_HP_Socket"));
	owner->GetWorldTimerManager().SetTimer(AimingTimer, this, &UPlayerWeaponComponent::Threaten, 0.3f, true, 0.0f);
	//owner->HPWidgetComponent
	//UGameplayStatics::PlaySoundAtLocation(this, owner->CharacterSound->aiming_start_Cue, GetOwner()->GetActorLocation());

	OnChangedAmmoUIDelegate.Broadcast();

}

void UPlayerWeaponComponent::StopAiming()
{
	if (CheckActorTag(owner, TEXT("Vehicle")))
	{
		OnVisibleAmmoUIDelegate.Broadcast();
		return;
	}
	Super::StopAiming();
	
	//owner->HPWidgetComponent->AttachToComponent(owner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("HP_Widget_Socket"));
	owner->GetWorldTimerManager().ClearTimer(AimingTimer);

	//UGameplayStatics::PlaySoundAtLocation(this, owner->CharacterSound->aiming_stop_Cue, GetOwner()->GetActorLocation());

	OnVisibleAmmoUIDelegate.Broadcast();
}

void UPlayerWeaponComponent::StartFire()
{
	if(curAmmo <= 0)
	{ 
		//UGameplayStatics::PlaySoundAtLocation(this, WeaponDataAsset->EmptySound_Cue, GetOwner()->GetActorLocation());
		owner->FindComponentByClass<UPlayerInputComponent>()->StartReload();
		return;
	}

	StopRcovery();
	Fire();
	Super::StartFire();


	startRot = owner->GetController()->GetControlRotation();

	if (weapontype == EWeaponType::Rifle || weapontype == EWeaponType::Heavy)
	{
		owner->GetWorldTimerManager().SetTimer(fHandle, this, &UPlayerWeaponComponent::Fire, m_firerate, true);
	}
}

void UPlayerWeaponComponent::StopFire()
{
	if (bFire)
	{
		Super::StopFire();
		if (weapontype == EWeaponType::Rifle || weapontype == EWeaponType::Heavy)
		{
			owner->GetWorldTimerManager().ClearTimer(fHandle);
		}
		owner->FindComponentByClass<UPlayerInputComponent>()->getInput()->IsFire = false;
		TotalPitchRecoilValue = 0.0f;
		StartRecovery();
	}
}

void UPlayerWeaponComponent::StartReload()
{
	//ReloadAmmo();
	StopFire();
	if (bReload)
	{
		return;
	}
	
	//UGameplayStatics::PlaySoundAtLocation(this, PlayerWeaponDataAsset->ReloadMagOutSound, owner->GetActorLocation());
	OnPlayReloadUIDelegate.ExecuteIfBound();
	bReload = CanReload();
}

void UPlayerWeaponComponent::StopReload()
{
	reloadvalue = 0;
	OnStopReloadUIDelegate.ExecuteIfBound();

	//UGameplayStatics::PlaySoundAtLocation(this, PlayerWeaponDataAsset->ReloadCliplockedSound, owner->GetActorLocation());
	bReload = false;
}

void UPlayerWeaponComponent::FinshReload()
{
	curAmmo = maxAmmo;
	OnChangedCrossHairAmmoDelegate.ExecuteIfBound();
	OnChangedAmmoUIDelegate.Broadcast();
	StopReload();
}

void UPlayerWeaponComponent::WeaponChange()
{
	InitData();
}

void UPlayerWeaponComponent::WeaponMeshSetting(UPlayerWeaponDataAsset* WeapondataAsset)
{
	if (WeapondataAsset)
	{
		ensureMsgf(WeapondataAsset->WeaponSkeletalMesh && WeapondataAsset->WeaponAnim, 
			TEXT("DA에 총 스켈레탈메쉬, 애니메이션 미할당"));

		WeaponMesh->SetSkeletalMesh(WeapondataAsset->WeaponSkeletalMesh);
		WeaponMesh->SetAnimInstanceClass(WeapondataAsset->WeaponAnim);
	}
}

void UPlayerWeaponComponent::ReloadTick(float Deltatime)
{
	return;
	if (bReload)
	{
		switch (weapontype)
		{
		case EWeaponType::Pistol:
			reloadCount += Deltatime * 6;
			break;
		case EWeaponType::Rifle:
			reloadCount += Deltatime * 20;
			break;

		default:
			reloadCount += Deltatime * 20;
			break;
		}

		if (reloadCount >= 1)
		{
			curAmmo++;
			reloadCount = 0;
			if (curAmmo == maxAmmo / 2)
			{
				UGameplayStatics::PlaySoundAtLocation(this, PlayerWeaponDataAsset->ReloadMagInSound, owner->GetActorLocation());
			}
			OnChangedCrossHairAmmoDelegate.ExecuteIfBound();
			OnChangedAmmoUIDelegate.Broadcast();
		}

		switch (weapontype)
		{
		case EWeaponType::Pistol:
			if (curAmmo == 10)
			{
				StopReload();
			}
			break;
		case EWeaponType::Rifle:
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
		case EWeaponType::Shotgun:
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
		if (bAiming && AimingRecoilValue > 0.0f)
		{
			yawRecoilValue = yawRecoilValue * AimingRecoilValue;
			pitchRecoilValue = pitchRecoilValue * AimingRecoilValue;
		}
	}
	else
	{
		yawRecoilValue = FMath::RandRange(yawRange.X, yawRange.Y);
		if (TotalPitchRecoilValue >= MaxPitchRecoilValue)
		{
			pitchRecoilValue = FMath::RandRange(pitchRange.X, pitchRange.Y);
		}
		else
		{
			pitchRecoilValue = 0.0f;
		}

		if (bAiming && AimingRecoilValue > 0.0f)
		{
			yawRecoilValue = yawRecoilValue * AimingRecoilValue;
			pitchRecoilValue = pitchRecoilValue * AimingRecoilValue;
		}
	}

	TotalPitchRecoilValue += pitchRecoilValue;
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
		UGameplayStatics::PlaySoundAtLocation(this, owner->CharacterSound->bullet_impacts_water_cue, result.Location, 0.5f);
		return;
	}

	FVector DecalSize(5.0f, 5.0f, 5.0f);
	FRotator DecalRotation = result.Normal.Rotation();
	FVector DecalLocation = result.Location;
	

	UDecalComponent* decal = UGameplayStatics::SpawnDecalAtLocation(result.GetActor(), PlayerWeaponDataAsset->BulletHole_Decals[0], DecalSize, DecalLocation, DecalRotation, 10.0f);
	if (decal)
	{
		decal->SetFadeScreenSize(0.0f);
		UGameplayStatics::PlaySoundAtLocation(this, owner->CharacterSound->bullet_impacts_concrete_cue, result.Location, 0.5f);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("decal_spawn"));
}

void UPlayerWeaponComponent::PlayRandomShotSound()
{
	Super::PlayRandomShotSound();
	float pitch = FMath::RandRange(0.9f, 1.2f);
	UGameplayStatics::PlaySoundAtLocation(this, PlayerWeaponDataAsset->FireSound, GetOwner()->GetActorLocation(), 1.0f, pitch);
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), owner->GetActorLocation(), 1.0f, owner, 0.0f, FName(TEXT("Shooting")));

}

void UPlayerWeaponComponent::PlayCameraShake(float scale)
{
	if (fireShake)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(fireShake, scale);
	}
}

void UPlayerWeaponComponent::SpawnField(FHitResult result)
{
	TSubclassOf<UObject> fieldbp = fieldActor->GeneratedClass;
	GetWorld()->SpawnActor<AActor>(fieldbp, m_result.Location, FRotator::ZeroRotator);
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

	if (GetWorld()->SweepSingleByChannel(result, start, end, FQuat::Identity, ECC_GameTraceChannel3, FCollisionShape::MakeSphere(50.0f)))
	{
		AActor* actor = result.GetActor();
		if (actor && actor->Tags.Num() > 0 && actor->ActorHasTag("Enemy")) {
			if (UStatComponent* stat = actor->FindComponentByClass<UStatComponent>()) {
				stat->bThreat = true;
			}
		}
	}


}





