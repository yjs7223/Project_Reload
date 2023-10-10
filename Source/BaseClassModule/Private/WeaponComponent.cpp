// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "Components/InputComponent.h"
#include "BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "CoverComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "HitImapactDataAsset.h"
#include "BaseWeaponDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Perception/AISense_Hearing.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Pakurable.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//m_CanShooting = true;	
	Weapon_Handle_R_Name = TEXT("hand_r_Socket");
	Weapon_Handle_L_Name = TEXT("hand_l_Socket");
	Arm_R_Name = TEXT("upperarm_r");
	Arm_L_Name = TEXT("upperarm_l");
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner<ABaseCharacter>();
	m_Cover = owner->FindComponentByClass<UCoverComponent>();
	
	TArray<UActorComponent*> pakurArr = owner->GetComponentsByInterface(UPakurable::StaticClass());
	if (pakurArr.Num() == 1) {
		m_PakurComp = pakurArr[0];
	}
	m_CanShooting = false;
	// ...
	
}

void UWeaponComponent::BeginDestroy()
{
	shootingAnimation.Clear();
	Dele_SpawnTrigger.Unbind();
	
	Super::BeginDestroy();
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	AimSetting();
	CalculateBlockingTick(DeltaTime);
	// ...
}

void UWeaponComponent::InitData()
{

}

void UWeaponComponent::SetAmmo(int p_ammo)
{
	holdAmmo = p_ammo;
	switch (weapontype)
	{
	case EWeaponType::Pistol:
		curAmmo = 10;
		break;
	case EWeaponType::Rifle:
		curAmmo = 30;
		break;
	case EWeaponType::Shotgun:
		curAmmo = 7;
		break;
	default:
		curAmmo = 30;
		break;
	}
}

void UWeaponComponent::CalculateWeaponHitLocation(float p_deltatime)
{

}

void UWeaponComponent::CalculateBlockingTick(float p_deltatime)
{
	if(!Cast<APlayerController>(owner->Controller)) return;
	
	FVector ViewPoint;
	FRotator cameraRotation;
	FHitResult result;
	FVector start;
	FVector end;

	owner->Controller->GetPlayerViewPoint(ViewPoint, cameraRotation);
	FCollisionQueryParams param(NAME_None, true, owner);

	start = owner->GetMesh()->GetSocketLocation("pelvis");

	start.Z += owner->GetDefaultHalfHeight() * 0.625f;
	if (m_Cover->IsPeeking()) {
		start += owner->GetActorRightVector() * 21.0f * m_Cover->FaceRight();
		//start = owner->GetMesh()->GetSocketLocation(Arm_R_Name);
	}

	end = ViewPoint + cameraRotation.Vector() * 15000.0f;
	//end = start + (cameraRotation.Vector() * 15000.0);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(),
		start,
		end,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		{ owner },
		EDrawDebugTrace::ForOneFrame,
		result, false);
	
	//DrawDebugSphere(GetWorld(), result.Location, 10, 32, FColor::Blue);
	if (result.bBlockingHit) {
		float distance = (owner->GetActorLocation() - result.Location).Length();
		//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("aaa : %f"), distance), true, false, FColor::Blue, p_deltatime);
		

		if (distance < m_WeaponDistance) {

			m_IsWeaponBlocking = true;
			return;
		}
		m_WeaponHitLocation = result.Location;
	}
	m_IsWeaponBlocking = false;
}

void UWeaponComponent::StartFire()
{
	bFire = true;
}

void UWeaponComponent::StopFire()
{
	bFire = false;
}

void UWeaponComponent::Fire()
{
	shootingAnimation.Broadcast();
	//fire
	return;
}

float UWeaponComponent::getAimYaw()
{
	return aimOffset.Yaw;
}

float UWeaponComponent::getAimPitch()
{
	return aimOffset.Pitch;
}

void UWeaponComponent::AimSetting()
{
	//if(GetOwner()->Tags.Num() == 0 || !GetOwner()->ActorHasTag(TEXT("Player"))) return;
	
	FRotator temprot;
	ACharacter* Owner = GetOwner<ACharacter>();
	temprot = Owner->GetControlRotation() - Owner->GetActorRotation();

	aimOffset.Pitch = FMath::ClampAngle(temprot.Pitch, -90, 90);
	aimOffset.Yaw = temprot.Yaw;
	if (aimOffset.Yaw > 180) aimOffset.Yaw -= 360;

	FRotator cameraRotation;
	FVector start;
	Owner->Controller->GetPlayerViewPoint(start, cameraRotation);
	FVector end = start + (cameraRotation.Vector() * 99999);

	FHitResult result;
	FCollisionQueryParams param(NAME_None, true, GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, param))
	{
		temprot = UKismetMathLibrary::FindLookAtRotation(result.Location, start);
	}
	else
	{
		temprot = UKismetMathLibrary::FindLookAtRotation(end, start);
	}


}

void UWeaponComponent::SetHandleing(bool isFaceRight, bool isCoverUse)
{
	FName handSocketName;
	FRotator meshRotate;
	FVector meshLocation;
	ACharacter* character = GetOwner<ACharacter>();
	if (isCoverUse) {
		if (UCoverComponent* cover = character->FindComponentByClass<UCoverComponent>()) {
			isFaceRight = cover->IsFaceRight();
		}
	}
	if (isFaceRight) {
		handSocketName = Weapon_Handle_R_Name;
		meshRotate = FRotator(0.0, 0.0, 0.0);
		meshLocation = WeaponMesh->GetRelativeLocation() * FVector(-1.0f, 1.0f, -1.0f);
	}
	else {
		handSocketName = Weapon_Handle_L_Name;
		meshRotate = FRotator(0.0, 180.0, 180.0);
		meshLocation = WeaponMesh->GetRelativeLocation() * FVector(-1.0f, 1.0f, -1.0f);
	}


	WeaponMesh->AttachToComponent(GetOwner<ACharacter>()->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, handSocketName);
	WeaponMesh->SetRelativeRotation(meshRotate);
	WeaponMesh->SetRelativeLocation(meshLocation);
}

void UWeaponComponent::PlayRandomShotSound()
{
	//float pitch = FMath::RandRange(0.9f, 1.2f);

	//UGameplayStatics::PlaySoundAtLocation(this, playerweapondata->FireSound, owner->GetActorLocation(), 1.0f, pitch);

	//UAISense_Hearing::ReportNoiseEvent(GetWorld(), owner->GetActorLocation(), 1.0f, owner, 0.0f, FName(TEXT("Shooting")));
}

void UWeaponComponent::SpawnImpactEffect(FHitResult result)
{
	if (HitImpactDataAsset)
	{
		UNiagaraSystem* hitFXNiagara = HitImpactDataAsset->MetalHitFXNiagara;

		if (CheckActorTag(result.GetActor(), TEXT("Enemy")) || CheckActorTag(result.GetActor(), TEXT("Player")))
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
				//hitFXNiagara = HitImpactDataAsset->RobotHitFXNiagara;
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


		FRotator m_rot = UKismetMathLibrary::FindLookAtRotation(result.Location, GetOwner()->GetActorLocation());
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, result.BoneName.ToString());
		m_rot.Pitch -= 90.0f;
		//hitFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), hitFXNiagara, result.Location, m_rot);
		UNiagaraComponent* hitFXComponent;
		if (!result.BoneName.IsNone())
		{
			USkeletalMeshComponent* mesh = result.GetActor()->FindComponentByClass<USkeletalMeshComponent>();
			if (mesh)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, hitFXComponent->GetAttachSocketName().ToString());
				
				hitFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(hitFXNiagara, mesh, result.BoneName, mesh->GetBoneLocation(result.BoneName), m_rot, FVector(.3f, .3f, .3f),EAttachLocation::KeepRelativeOffset, true,ENCPoolMethod::None);

			}
		}
		else
		{
			hitFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), hitFXNiagara, result.Location, m_rot);
		}
	}
}

float UWeaponComponent::CalcDamage(FHitResult result, FVector2D p_damage)
{
	if (MaxRange > 0)
	{
		FVector range = result.Location - owner->GetActorLocation();
		float alpha = range.Length() / MaxRange;
		alpha = FMath::Clamp(alpha, 0.0f, 1.0f);
		float m_dmg = FMath::Lerp(p_damage.X, p_damage.Y, alpha);
		m_dmg += FMath::RandRange(-Deviation, Deviation);
		return m_dmg;
	}
	return 0.0f;
}

bool UWeaponComponent::CheckActorTag(AActor* actor, FName tag)
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

bool UWeaponComponent::IsWeaponBlocking()
{
	return m_IsWeaponBlocking;
}

bool UWeaponComponent::IsAiming()
{
	return bAiming && !IsWeaponBlocking();
}

bool UWeaponComponent::IsFireing()
{
	return bFire && !IsWeaponBlocking();
}

FVector UWeaponComponent::getWeaponHitLocation()
{
	return m_WeaponHitLocation;
}

bool UWeaponComponent::IsUsingWeapon()
{
	bool isPakuru = false;
	
	if (m_PakurComp && m_PakurComp->GetClass()) {
		isPakuru = IPakurable::Execute_IsRolling(m_PakurComp);
	} 

	return (IsAiming() || IsFireing()) && !isPakuru;
}
