// Fill out your copyright notice in the Description page of Project Settings.


#include "AIWeaponComponent.h"
#include "AICharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "StatComponent.h"
#include "ST_AIShot.h"
#include <Kismet/GameplayStatics.h>
#include "ST_Spawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "AICommander.h"
#include "SubEncounterSpace.h"
#include "AISpawner.h"
#include "HitImapactDataAsset.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI_Controller.h"
#include "AIWeaponDataAsset.h"
#include "Engine/EngineTypes.h"

UAIWeaponComponent::UAIWeaponComponent()
{
	// 데이터 테이블 삽입
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/AI_Project/DT/DT_AIShot.DT_AIShot'"));
	if (DataTable.Succeeded())
	{
		AIShotData = DataTable.Object;
	}

	//// 총알 나이아가라 삽입
	//static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ShotFXNiagara(TEXT("NiagaraSystem'/Game/SGJ/NS_BulletProjectile.NS_BulletProjectile'"));
	//if (DataTable.Succeeded())
	//{
	//	shotFXNiagara = ShotFXNiagara.Object;
	//}

	// 라이플
	static ConstructorHelpers::FObjectFinder<UDataAsset> rifle_da(TEXT("AIWeaponDataAsset'/Game/AI_Project/AI_Pakage/BaseAI/DA/DA_AIRifle.DA_AIRifle'"));
	if (rifle_da.Succeeded())
	{
		RifleDataAsset = Cast<UAIWeaponDataAsset>(rifle_da.Object);
	}
	// 스나이퍼
	static ConstructorHelpers::FObjectFinder<UDataAsset> sniper_da(TEXT("AIWeaponDataAsset'/Game/AI_Project/AI_Pakage/BaseAI/DA/DA_AISniper.DA_AISniper'"));
	if (sniper_da.Succeeded())
	{
		SniperDataAsset = Cast<UAIWeaponDataAsset>(sniper_da.Object);
	}
	// 헤비
	static ConstructorHelpers::FObjectFinder<UDataAsset> heavy_da(TEXT("AIWeaponDataAsset'/Game/AI_Project/AI_Pakage/BaseAI/DA/DA_AIHeavy.DA_AIHeavy'"));
	if (heavy_da.Succeeded())
	{
		HeavyDataAsset = Cast<UAIWeaponDataAsset>(heavy_da.Object);
	}

	// 총 피격 이펙트
	static ConstructorHelpers::FObjectFinder<UDataAsset> hitimpact(TEXT("HitImapactDataAsset'/Game/yjs/DA_HItImapct.DA_HItImapct'"));
	if (hitimpact.Succeeded())
	{
		HitImpactDataAsset = Cast<UHitImapactDataAsset>(hitimpact.Object);
	}
}

void UAIWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = Cast<AAICharacter>(GetOwner());
	commander = Cast<AAICommander>(UGameplayStatics::GetActorOfClass(GetWorld(), AAICommander::StaticClass()));

	use_Shot_State = true;

	player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	playerMesh = player->FindComponentByClass<USkeletalMeshComponent>();
	blackboardTarget = Cast<AActor>(Cast<AAI_Controller>(owner->GetController())->GetBlackboardComponent()->GetValueAsObject("Target"));

	GetOwner()->GetWorldTimerManager().ClearTimer(timer);
	GetOwner()->GetWorldTimerManager().SetTimer(timer, this, &UAIWeaponComponent::CheckTrace, 1, true, 0.0f);
}


// Called every frame
void UAIWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*if (shot_State)
	{
		ShotAITimer(DeltaTime);
	}*/
	// ...
}

void UAIWeaponComponent::ShotAI()
{
	owner->bUseControllerRotationYaw = true;
	
	FVector loc;
	FRotator rot;
	owner->GetController()->GetPlayerViewPoint(loc, rot);

	float x = 0, y = 0;
	
	x = FMath::RandRange(-recoil_Radius, recoil_Radius);
	y = FMath::RandRange(-recoil_Radius, recoil_Radius);

	FVector start = WeaponMesh->GetSocketLocation(TEXT("MuzzleFlashSocket"));
	FVector playerLocation = playerMesh->GetSocketLocation(TEXT("spine_04"));

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
	if (GetWorld()->LineTraceSingleByChannel(m_result, start, playerLocation, ECC_Visibility, traceParams))
	{
		rot = UKismetMathLibrary::FindLookAtRotation(start, m_result.Location);
		// AI가 앞을 막고 있을 때 사격 불가능
		if (m_result.GetActor()->ActorHasTag("Enemy"))
		{
			return;
		}
	}
	
	// 사격 방향 체크
	if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_Visibility, traceParams))
	{
		if (m_result.GetActor()->ActorHasTag("Player"))
		{
			//bb->SetValueAsVector(name, hitr.Location);
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Name : bbb")));
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Name : %s"), *hit.GetComponent()->GetName()));

			float deviation = FMath::RandRange((*curAIShotData).Shot_Deviation, -(*curAIShotData).Shot_Deviation);
			auto temp = m_result.GetActor()->FindComponentByClass<UStatComponent>();
			if (temp) {
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("actor1 : %s"), *temp->GetName()));
				float dmg = shot_MaxDmg - (shot_MaxDmg - shot_MinDmg) *
					((owner->GetDistanceTo(GetWorld()->GetFirstPlayerController()->GetPawn()) - shot_MinRange) / (shot_MaxRange - shot_MinRange))
					+ deviation;

				temp->Attacked(dmg, GetOwner<ABaseCharacter>());
			}
		}

		AISpawnImpactEffect(m_result);
		rot = UKismetMathLibrary::FindLookAtRotation(start, m_result.Location);
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

	//DrawDebugLine(GetWorld(), start, end, FColor::Orange, false, 0.1f);

	// 총구 불꽃 생성
	//UGameplayStatics::SpawnEmitterAtLocation(this, MuzzleFireParticle, start, rot, true);
	UGameplayStatics::SpawnEmitterAttached(MuzzleFireParticle, WeaponMesh, FName("MuzzleFlashSocket"));

	// 총알 생성
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletTracerParticle, start, rot);

	// 사운드 재생
	PlayRandomShotSound();

	// 총알 생성

	/*shotFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, shotFXNiagara, start, rot + FRotator(x, y, 0));

	shotFXComponent->SetNiagaraVariableVec3("BeamEnd", end2);*/

	//DrawDebugLine(GetWorld(), start, end, FColor::Orange, false, 0.1f);
	//name = "AttackLocation";
}

void UAIWeaponComponent::ShotAITimer(float t)
{
	cur_Shot_Delay += t;
	if (cur_Shot_Delay >= shot_Delay)
	{
		ShotAI();
		cur_Shot_Delay = 0;
	}
}

void UAIWeaponComponent::ShotAIStart()
{
	shot_State = true;

	// 총 공격수만큼 사격했다면 사격 상태 해제
	if (cur_Shot_Count <= 0)
	{
		shot_State = false;
		use_Shot_State = false;
		recoil_Radius = recoilMax_Radius;
	}
}

void UAIWeaponComponent::ShotAIStop()
{
	shot_State = false;
	use_Shot_State = true;
	cur_Shot_Count = shot_MaxCount;
	recoil_Radius = recoilMax_Radius;
}

void UAIWeaponComponent::ReloadAI()
{
	cur_Shot_Count = shot_MaxCount;
	use_Shot_State = true;
}

void UAIWeaponComponent::SetDataTable(FName EnemyName)
{
	if (AIShotData)
	{
		// 데이터 가져오기
		curAIShotData = AIShotData->FindRow<FST_AIShot>(EnemyName, TEXT(""));
		switch (Cast<AAICharacter>(owner)->type)
		{
		case Enemy_Name::RIFLE:
			AIWeaponDataAsset = RifleDataAsset;
			break;
		case Enemy_Name::SNIPER:
			AIWeaponDataAsset = SniperDataAsset;
			break;
		case Enemy_Name::HEAVY:
			AIWeaponDataAsset = HeavyDataAsset;
			break;
		}

		// 가져온 데이터 삽입
		recoil_Range = curAIShotData->Recoil_Range;
		recoilMax_Radius = curAIShotData->RecoilMax_Radius;
		recoilMin_Radius = curAIShotData->RecoilMin_Radius;

		shot_MaxRange = curAIShotData->Shot_MaxRange;
		shot_MinRange = curAIShotData->Shot_MinRange;

		shot_MaxDmg = curAIShotData->Shot_MaxDmg;
		shot_MinDmg = curAIShotData->Shot_MinDmg;

		shot_MaxCount = curAIShotData->Shot_MaxCount;

		shot_Delay = curAIShotData->Shot_ShootDelay;

		// 현재 반동은 최대로 시작
		recoil_Radius = recoilMax_Radius;

		// 첫 총알은 최대로
		cur_Shot_Count = shot_MaxCount;
	}
	if (AIWeaponDataAsset != nullptr)
	{
		MuzzleFireParticle = AIWeaponDataAsset->MuzzleFireParticle;
		BulletTracerParticle = AIWeaponDataAsset->BulletTracerParticle;
		shotFXNiagara = AIWeaponDataAsset->BulletTrailFXNiagara;

		ShotSounds = AIWeaponDataAsset->ShotSounds;

		Decal = AIWeaponDataAsset->Decals[0];
	}
}

bool UAIWeaponComponent::AITypeSniperCheck()
{
	if (Cast<AAICharacter>(GetOwner())->type == Enemy_Name::SNIPER)
	{
		return true;
	}
	return false;
}

void UAIWeaponComponent::CheckTrace()
{
	if (commander == nullptr) return;
	if (commander->Now_suben == nullptr) return;
	if (commander->Now_suben->spawn == nullptr) return;
	if (commander->Now_suben->spawn->cpyLastPoint == nullptr) return;
	if (!Cast<AAI_Controller>(owner->GetController())->GetBlackboardComponent()->GetValueAsBool("AI_Active")) return;

	FCollisionQueryParams collisionParams;
	FVector start = WeaponMesh->GetSocketLocation(TEXT("MuzzleFlashSocket"));

	if (GetWorld()->LineTraceSingleByChannel(result, start, commander->m_suben->spawn->cpyLastPoint->GetActorLocation(), ECC_Visibility, collisionParams))
	{
		if (result.GetActor()->ActorHasTag("Last"))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("CheckTrace()"));
			GetWorld()->DestroyActor(result.GetActor());
		}
	}

	//DrawDebugLine(GetWorld(), start, commander->m_suben->spawn->cpyLastPoint->GetActorLocation(), FColor::Red, false, 0.1f);
}

void UAIWeaponComponent::AISpawnImpactEffect(FHitResult p_result)
{
	if (HitImpactDataAsset)
	{
		if (p_result.GetActor())
		{
			if (p_result.GetActor()->Tags.Num() > 0)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("SpawnImpact"));
				if (p_result.GetActor()->ActorHasTag("Player"))
				{
					if (p_result.GetActor()->ActorHasTag("Robot"))
					{
						hitFXNiagara = HitImpactDataAsset->RobotHitFXNiagara;
					}
					else if (p_result.GetActor()->ActorHasTag("Human"))
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
					if (p_result.GetActor()->ActorHasTag("Metal"))
					{
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Metal"));
						hitFXNiagara = HitImpactDataAsset->MetalHitFXNiagara;
					}
					else if (p_result.GetActor()->ActorHasTag("Rock"))
					{
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Rock"));
						hitFXNiagara = HitImpactDataAsset->RockHitFXNiagara;
					}
					else if (p_result.GetActor()->ActorHasTag("Mud"))
					{
						hitFXNiagara = HitImpactDataAsset->MudHitFXNiagara;
					}
					else if (p_result.GetActor()->ActorHasTag("Glass"))
					{
						hitFXNiagara = HitImpactDataAsset->GlassHitFXNiagara;
					}
					else if (p_result.GetActor()->ActorHasTag("Water"))
					{
						hitFXNiagara = HitImpactDataAsset->WaterHitFXNiagara;
					}
					else
					{
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("default"));
						hitFXNiagara = HitImpactDataAsset->MetalHitFXNiagara;
					}

				}
			}
			else
			{
				hitFXNiagara = HitImpactDataAsset->MetalHitFXNiagara;
			}
		}
	}

	FRotator m_rot = UKismetMathLibrary::FindLookAtRotation(p_result.Location, GetOwner()->GetActorLocation());
	m_rot.Pitch -= 90.0f;

	hitFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, hitFXNiagara, p_result.Location);
}

void UAIWeaponComponent::PlayRandomShotSound()
{
	int r = FMath::RandRange(0, 3);
	UGameplayStatics::PlaySoundAtLocation(this, ShotSounds[r], GetOwner()->GetActorLocation());
}
