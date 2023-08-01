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

UAIWeaponComponent::UAIWeaponComponent()
{
	// 데이터 테이블 삽입
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/AI_Project/DT/DT_AIShot.DT_AIShot'"));
	if (DataTable.Succeeded())
	{
		AIShotData = DataTable.Object;
	}

	// 총구 불꽃 파티클 삽입
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ShotFX(TEXT("ParticleSystem'/Game/ThirdPersonKit/Particles/P_RealAssaultRifle_MF.P_AssaultRifle_MF'"));
	if (DataTable.Succeeded())
	{
		shotFX = ShotFX.Object;
	}

	//// 총알 나이아가라 삽입
	//static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ShotFXNiagara(TEXT("NiagaraSystem'/Game/SGJ/NS_BulletProjectile.NS_BulletProjectile'"));
	//if (DataTable.Succeeded())
	//{
	//	shotFXNiagara = ShotFXNiagara.Object;
	//}
	//use_Shot_State = true;

	//// 라이플
	//static ConstructorHelpers::FObjectFinder<UDataAsset> rifle_da(TEXT("WeaponDataAsset'/Game/yjs/DA_Rifle.DA_Rifle'"));
	//if (rifle_da.Succeeded())
	//{

	//	RifleDataAssets = Cast<UWeaponDataAsset>(rifle_da.Object);
	//}
}

void UAIWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = Cast<AAICharacter>(GetOwner());
	commander = Cast<AAICommander>(UGameplayStatics::GetActorOfClass(GetWorld(), AAICommander::StaticClass()));

	AITypeSetting();
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

	CheckTrace();
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
	FVector end = start + ((rot + FRotator(x, y, 0)).Vector() * shot_MaxRange);
	FVector end2 = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FCollisionQueryParams traceParams;

	// 조준 방향 체크
	if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_Visibility, traceParams))
	{
		// AI가 앞을 막고 있을 때 사격 불가능
		if (m_result.GetActor()->ActorHasTag("Enemy"))
		{
			return;
		}
	}
	
	// 사격 방향 체크
	if (GetWorld()->LineTraceSingleByChannel(m_result, start, end2, ECC_Visibility, traceParams))
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
				temp->Attacked(shot_MaxDmg - (shot_MaxDmg - shot_MinDmg) * 
					((owner->GetDistanceTo(GetWorld()->GetFirstPlayerController()->GetPawn()) - shot_MinRange) / (shot_MaxRange - shot_MinRange)) 
					+ deviation);
			}
		}

		AISpawnImpactEffect(m_result);
	}

	// 점점 반동이 줄어듦
	if (recoil_Radius > recoilMin_Radius)
	{
		recoil_Radius = recoilMax_Radius - recoilMin_Radius / cur_Shot_Count;
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
	UGameplayStatics::SpawnEmitterAtLocation(this, shotFX, start, rot, true);

	// 총알 생성
	shotFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, shotFXNiagara, start, rot + FRotator(x, y, 0));

	shotFXComponent->SetNiagaraVariableVec3("BeamEnd", end2);

	DrawDebugLine(GetWorld(), start, end2, FColor::Orange, false, 0.1f);
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

void UAIWeaponComponent::AITypeSetting()
{
	switch (type)
	{
	case Enemy_Name::RIFLE:
		// 라이플 데이터 가져오기
		curAIShotData = AIShotData->FindRow<FST_AIShot>("Rifle_E", TEXT(""));
		break;
	case Enemy_Name::HEAVY:
		// 라이플 데이터 가져오기
		curAIShotData = AIShotData->FindRow<FST_AIShot>("Heavy_E", TEXT(""));
		break;
	case Enemy_Name::SNIPER:
		// 라이플 데이터 가져오기
		curAIShotData = AIShotData->FindRow<FST_AIShot>("Sniper_E", TEXT(""));
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

bool UAIWeaponComponent::AITypeSniperCheck()
{
	if (type == Enemy_Name::SNIPER)
	{
		return true;
	}
	return false;
}

void UAIWeaponComponent::CheckTrace()
{
	FCollisionQueryParams collisionParams;
	FVector start = WeaponMesh->GetSocketLocation(TEXT("MuzzleFlashSocket"));

	if (commander == nullptr) return;
	if (commander->m_suben == nullptr) return;
	if (commander->m_suben->spawn == nullptr) return;
	if (commander->m_suben->spawn->cpyLastPoint == nullptr) return;
	if (!Cast<AAI_Controller>(owner->GetController())->GetBlackboardComponent()->GetValueAsBool("AI_Active")) return;

	if (GetWorld()->LineTraceSingleByChannel(result, start, commander->m_suben->spawn->cpyLastPoint->GetActorLocation(), ECC_Visibility, collisionParams))
	{
		if (result.GetActor()->ActorHasTag("Last"))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("CheckTrace()"));
			GetWorld()->DestroyActor(result.GetActor());
		}
	}

	DrawDebugLine(GetWorld(), start, commander->m_suben->spawn->cpyLastPoint->GetActorLocation(), FColor::Orange, false, 0.1f);
}

void UAIWeaponComponent::AISpawnImpactEffect(FHitResult p_result)
{
	if (HitImpactDataAsset)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("SpawnImpact"));
		if (result.GetActor()->ActorHasTag("Enemy"))
		{
			if (result.GetActor()->ActorHasTag("Robot"))
			{
				hitFXNiagara = HitImpactDataAsset->RobotHitFXNiagara;
			}
			else if (result.GetActor()->ActorHasTag("Human"))
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
			if (result.GetActor()->ActorHasTag("Metal"))
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Metal"));
				hitFXNiagara = HitImpactDataAsset->MetalHitFXNiagara;
			}
			else if (result.GetActor()->ActorHasTag("Rock"))
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Rock"));
				hitFXNiagara = HitImpactDataAsset->RockHitFXNiagara;
			}
			else if (result.GetActor()->ActorHasTag("Mud"))
			{
				hitFXNiagara = HitImpactDataAsset->MudHitFXNiagara;
			}
			else if (result.GetActor()->ActorHasTag("Glass"))
			{
				hitFXNiagara = HitImpactDataAsset->GlassHitFXNiagara;
			}
			else if (result.GetActor()->ActorHasTag("Water"))
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

	hitFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, hitFXNiagara, result.Location);
}
