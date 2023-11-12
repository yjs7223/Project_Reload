// Fill out your copyright notice in the Description page of Project Settings.


#include "AIWeaponComponent.h"
#include "AICharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "StatComponent.h"
#include "AIWeaponData.h"
#include "AIWeaponDataAsset.h"
#include "AIStatComponent.h"
#include <Kismet/GameplayStatics.h>
#include "ST_Spawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "AICommander.h"
#include "EncounterSpace.h"
#include "AISpawner.h"
#include "HitImapactDataAsset.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI_Controller.h"
#include "AIWeaponDataAsset.h"
#include "Engine/EngineTypes.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Bullet.h"
#include "EmptyShellSpawnable.h"
#include "Components/SpotLightComponent.h"
#include "NiagaraComponent.h"
#include "AIInputComponent.h"

UAIWeaponComponent::UAIWeaponComponent()
{
	// 데이터 테이블 삽입
	DT_AIWeaponData = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/AI_Project/DT/DT_AIWeapon.DT_AIWeapon'"));
}

void UAIWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner<AAICharacter>();
	commander = Cast<AAICommander>(UGameplayStatics::GetActorOfClass(GetWorld(), AAICommander::StaticClass()));

	use_Shot_State = true;
	Cast<AAI_Controller>(owner->GetController())->GetBlackboardComponent()->SetValueAsBool("AI_UseShot", true);
	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) != nullptr)
	{
		player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}
	playerMesh = player->FindComponentByClass<USkeletalMeshComponent>();
	blackboardTarget = Cast<AActor>(Cast<AAI_Controller>(owner->GetController())->GetBlackboardComponent()->GetValueAsObject("Target"));

	if (owner->FindComponentByClass<UNiagaraComponent>())
	{
		laserFXComponent = owner->FindComponentByClass<UNiagaraComponent>();
		//AimFlashFXComponent = owner->FindComponentByClass<UNiagaraComponent>();
	}

	owner->GetWorldTimerManager().ClearTimer(timer);
	owner->GetWorldTimerManager().SetTimer(timer, this, &UAIWeaponComponent::CheckTrace, 1, true, 0.0f);

	//라이트
	//SpotLightCmp->AttachToComponent(WeaponMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("LaserSightSocket"));



}


// Called every frame
void UAIWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	/*if (Cast<AAI_Controller>(owner->GetController())->GetBlackboardComponent()->GetValueAsBool("AI_InCover")) {
		owner->GetController()->SetControlRotation(UKismetMathLibrary::FindLookAtRotation(owner->GetActorLocation(),player->GetActorLocation()));

	}*/

	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	blackboardTarget = Cast<AActor>(Cast<AAI_Controller>(owner->GetController())->GetBlackboardComponent()->GetValueAsObject("Target"));
	/*if (shot_State)
	{
		ShotAITimer(DeltaTime);
	}*/
	// ...
}


void UAIWeaponComponent::Fire()
{
	FVector loc;
	FRotator rot;
	owner->GetController()->GetPlayerViewPoint(loc, rot);
	owner->FindComponentByClass<UAIInputComponent>()->AIStopRuning();
	float x = 0, y = 0;

	x = FMath::RandRange(-recoil_Radius, recoil_Radius);
	y = FMath::RandRange(-recoil_Radius, recoil_Radius);

	FVector start = WeaponMesh->GetSocketLocation(TEXT("MuzzleFlashSocket"));
	FVector playerLocation = playerMesh->GetSocketLocation(TEXT("spine_05"));
	FVector end = start + ((playerLocation - start).Rotation() + FRotator(x, y, 0)).Vector() * shot_MaxRange;

	if (blackboardTarget != nullptr)
	{
		// 타겟이 플레이어가 아니면
		if (blackboardTarget != player)
		{
			end = start + ((blackboardTarget->GetActorLocation() - start).Rotation() + FRotator(x, y, 0)).Vector() * shot_MaxRange;
		}
	}

	FCollisionQueryParams traceParams;

	// 조준 방향 체크
	if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_Visibility, traceParams))
	{
		rot = UKismetMathLibrary::FindLookAtRotation(start, m_result.Location);
		// AI가 앞을 막고 있을 때 사격 불가능
		if (m_result.GetActor()->ActorHasTag("Enemy"))
		{
			return;
		}
	}

	// 사격 방향 체크
	if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_GameTraceChannel6, traceParams))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, m_result.GetActor()->GetName());
		if (m_result.GetActor()->ActorHasTag("Player"))
		{
			auto temp = m_result.GetActor()->FindComponentByClass<UStatComponent>();
			if (temp) {
				float dmg = CalcDamage(m_result, damage);

				temp->Attacked(dmg, GetOwner<ABaseCharacter>());
				temp->hitNormal = m_result.ImpactNormal;
			}
		}
		SpawnImpactEffect(m_result);
		rot = UKismetMathLibrary::FindLookAtRotation(start, m_result.Location);
	}
	// 사운드 재생
	PlayRandomShotSound();
	Super::Fire();
	UAnimInstance* animinstatce = WeaponMesh->GetAnimInstance();
	if (animinstatce && animinstatce->GetClass()->ImplementsInterface(UEmptyShellSpawnable::StaticClass())) {
		IEmptyShellSpawnable::Execute_EmptyShellSpawn((animinstatce));
	}

	ABullet* bullet = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), start, rot);
	if (bullet)
	{
		bullet->SpawnBulletFx(AIWeaponDataAsset->BulletTrailFXNiagara, rot.Vector(), owner, AIWeaponDataAsset->Bullet_Passby_Sound);
		bullet->OnBulletHitDelegate.AddUObject(this, &UAIWeaponComponent::SpawnImpactEffect);
	}

	// 점점 반동이 줄어듦
	if (recoil_Radius > recoilMin_Radius)
	{
		recoil_Radius -= (recoilMax_Radius - recoilMin_Radius) / shot_MaxCount;
	}
	else
	{
		// 최소로 고정
		recoil_Radius = recoilMin_Radius;
	}

	// 총알 감소
	cur_Shot_Count--;


	// 총구 불꽃 생성
	UGameplayStatics::SpawnEmitterAttached(MuzzleFireParticle, WeaponMesh, FName("MuzzleFlashSocket"));

	

}

void UAIWeaponComponent::ShotAITimer(const float t)
{
	cur_fire_Rate += t;
	if (cur_fire_Rate >= fire_Rate)
	{
		Fire();
		cur_fire_Rate = 0;
	}
}



void UAIWeaponComponent::StartFire()
{
	bFire = true;

	// 총 공격수만큼 사격했다면 사격 상태 해제
	if (cur_Shot_Count <= 0)
	{
		bFire = false;
		use_Shot_State = false;
		recoil_Radius = recoilMax_Radius;
	}
}

void UAIWeaponComponent::StopFire()
{
	bFire = false;

	use_Shot_State = true;
	cur_Shot_Count = shot_MaxCount;
	recoil_Radius = recoilMax_Radius;

	LaserOff();
}

void UAIWeaponComponent::ReloadAI()
{
	cur_Shot_Count = shot_MaxCount;
	use_Shot_State = true;
}


void UAIWeaponComponent::InitData()
{
	if (DT_AIWeaponData)
	{
		// 데이터 가져오기
		switch (owner->FindComponentByClass<UAIStatComponent>()->type)
		{
		case Enemy_Name::RIFLE:
			curAIWeaponData = DT_AIWeaponData->FindRow<FAIWeaponStruct>("Rifle_E", TEXT(""));
			AIWeaponDataAsset = LoadObject<UAIWeaponDataAsset>(NULL, TEXT("AIWeaponDataAsset'/Game/AI_Project/AI_Pakage/BaseAI/DA/DA_AIRifle.DA_AIRifle'"));
			break;
		case Enemy_Name::SNIPER:
			curAIWeaponData = DT_AIWeaponData->FindRow<FAIWeaponStruct>("Sniper_E", TEXT(""));
			AIWeaponDataAsset = LoadObject<UAIWeaponDataAsset>(NULL, TEXT("AIWeaponDataAsset'/Game/AI_Project/AI_Pakage/BaseAI/DA/DA_AISniper.DA_AISniper'"));
			break;
		case Enemy_Name::HEAVY:
			curAIWeaponData = DT_AIWeaponData->FindRow<FAIWeaponStruct>("Heavy_E", TEXT(""));
			AIWeaponDataAsset = LoadObject<UAIWeaponDataAsset>(NULL, TEXT("AIWeaponDataAsset'/Game/AI_Project/AI_Pakage/BaseAI/DA/DA_AIHeavy.DA_AIHeavy'"));
			break;
		}
		if (curAIWeaponData)
		{
			// 가져온 데이터 삽입
			recoil_Range = curAIWeaponData->Recoil_Range;
			recoilMax_Radius = curAIWeaponData->RecoilMax_Radius;
			recoilMin_Radius = curAIWeaponData->RecoilMin_Radius;

			shot_MaxRange = curAIWeaponData->Max_Range;

			damage.X = curAIWeaponData->Max_Damage;
			damage.Y = curAIWeaponData->Min_Damage;

			shot_MaxCount = curAIWeaponData->Max_FireCount;

			fire_Rate = curAIWeaponData->Fire_Rate;

			// 현재 반동은 최대로 시작
			recoil_Radius = recoilMax_Radius;

			// 첫 총알은 최대로
			cur_Shot_Count = shot_MaxCount;
		}
	}
	if (AIWeaponDataAsset != nullptr)
	{
		if (AIWeaponDataAsset->WeaponSkeletalMesh)
		{
			WeaponMesh->SetSkeletalMesh(AIWeaponDataAsset->WeaponSkeletalMesh);
		}

		if (AIWeaponDataAsset->WeaponAnim)
		{
			WeaponMesh->SetAnimInstanceClass(AIWeaponDataAsset->WeaponAnim);
		}

		MuzzleFireParticle = AIWeaponDataAsset->MuzzleFireParticle;

		FireSound = AIWeaponDataAsset->FireSound;

		Decal = AIWeaponDataAsset->BulletHole_Decals[0];
	}
}

void UAIWeaponComponent::PlayRandomShotSound()
{
	float pitch = FMath::RandRange(1.3f, 1.5f);
	int sound_Select = FMath::RandRange(0, AIWeaponDataAsset->Arr_AIShotSound.Num() - 1);
	if (owner->FindComponentByClass<UAIStatComponent>()->type == Enemy_Name::HEAVY || owner->FindComponentByClass<UAIStatComponent>()->type == Enemy_Name::RIFLE)
	{
		UGameplayStatics::PlaySoundAtLocation(this, AIWeaponDataAsset->Arr_AIShotSound[sound_Select], owner->GetActorLocation(), 0.5f, pitch);
	}
	else if (owner->FindComponentByClass<UAIStatComponent>()->type == Enemy_Name::SNIPER)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, owner->GetActorLocation(), 1.0f, pitch);
	}
	
	
}

bool UAIWeaponComponent::AITypeSniperCheck()
{
	if (owner->FindComponentByClass<UAIStatComponent>()->type == Enemy_Name::SNIPER)
	{
		return true;
	}
	return false;
}

void UAIWeaponComponent::CheckTrace()
{
	if (commander == nullptr) return;
	if (commander->Now_en == nullptr) return;
	if (commander->Now_en->spawn == nullptr) return;
	if (commander->Now_en->spawn->cpyLastPoint == nullptr) return;
	if (owner->FindComponentByClass<UAIStatComponent>()->combat == CombatState::PATROL) return;
	if (!Cast<AAI_Controller>(owner->GetController())->GetBlackboardComponent()->GetValueAsBool("AI_Active")) return;

	FCollisionQueryParams collisionParams;
	FVector start = WeaponMesh->GetSocketLocation(TEXT("MuzzleFlashSocket"));

	if (GetWorld()->LineTraceSingleByChannel(result, start, commander->Now_en->spawn->cpyLastPoint->GetActorLocation(), ECC_Visibility, collisionParams))
	{
		if (result.GetActor()->ActorHasTag("Last"))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("CheckTrace()"));
			GetWorld()->DestroyActor(result.GetActor());
		}
	}
}


void UAIWeaponComponent::LaserOn()
{
	if (owner->FindComponentByClass<UAIStatComponent>()->type == Enemy_Name::SNIPER)
	{
		FVector start = WeaponMesh->GetSocketLocation(TEXT("MuzzleFlashSocket"));
		FVector end = playerMesh->GetSocketLocation(TEXT("spine_05"));

		if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_GameTraceChannel6, {}))
		{
			end = m_result.Location;;
		}

		laserFXComponent->SetNiagaraVariableVec3(TEXT("Beam Start"), start);
		laserFXComponent->SetNiagaraVariableVec3(TEXT("Beam End"), end);
	}
}

void UAIWeaponComponent::LaserOff()
{
	if (owner->FindComponentByClass<UAIStatComponent>()->type == Enemy_Name::SNIPER)
	{
		if (laserFXComponent)
		{
			laserFXComponent->SetNiagaraVariableVec3("Beam Start", FVector(0, 0, 0));
			laserFXComponent->SetNiagaraVariableVec3("Beam End", FVector(0, 0, 0));
		}
	}
}

void UAIWeaponComponent::AimFalshOn()
{
	if (owner->FindComponentByClass<UAIStatComponent>()->type == Enemy_Name::SNIPER)
	{
		//UGameplayStatics::SpawnEmitterAttached(MuzzleFireParticle, WeaponMesh, FName("AttachmentSocketScope"));

		AimFlashFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(AimFlashFXNiagara, WeaponMesh, TEXT("AttachmentSocketScope"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
		if (AimFlashFXComponent)
		AimFlashFXComponent->SetActive(true);
	}

}

void UAIWeaponComponent::AimFalshOff()
{

	if (owner->FindComponentByClass<UAIStatComponent>()->type == Enemy_Name::SNIPER)
	{
		if(AimFlashFXComponent)
			AimFlashFXComponent->SetActive(false);
	}
}
