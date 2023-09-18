// Fill out your copyright notice in the Description page of Project Settings.


#include "BossWeaponComponent.h"
#include "StatComponent.h"
#include "BaseCharacter.h"
#include "MyBossCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "Kismet/KismetMathLibrary.h"
#include "StatComponent.h"

// Sets default values for this component's properties
UBossWeaponComponent::UBossWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBossWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner<AMyBossCharacter>();

	player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	playerMesh = player->FindComponentByClass<USkeletalMeshComponent>();
}


// Called every frame
void UBossWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBossWeaponComponent::TakeDamage(AActor* p_TargetActor, float p_Damage)
{
	auto target = p_TargetActor->FindComponentByClass<UStatComponent>();
	if (target)
	{
		target->Attacked(p_Damage);
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, GetOwner<ABaseCharacter>()->GetName());
		//target->Attacked(p_Damage);
	}
}

void UBossWeaponComponent::Fire()
{
	FVector loc;
	FRotator rot;
	owner->GetController()->GetPlayerViewPoint(loc, rot);

	float x = 0, y = 0;

	x = FMath::RandRange(-recoil_Radius, recoil_Radius);
	y = FMath::RandRange(-recoil_Radius, recoil_Radius);

	//머즐 추가해서 이름 변경하기
	FVector start = WeaponMesh->GetSocketLocation(TEXT("MuzzleFlashSocket"));
	FVector playerLocation = playerMesh->GetSocketLocation(TEXT("spine_05"));

	FVector end = start + ((playerLocation - start).Rotation() + FRotator(x, y, 0)).Vector() * shot_MaxRange;

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
	if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_GameTraceChannel6, traceParams))
	{
		if (m_result.GetActor()->ActorHasTag("Player"))
		{
			auto temp = m_result.GetActor()->FindComponentByClass<UStatComponent>();
			if (temp) {
				float dmg = 0;

				temp->Attacked(dmg, GetOwner<ABaseCharacter>());
				temp->hitNormal = m_result.ImpactNormal;
			}
		}
		//SpawnImpactEffect(m_result);
		rot = UKismetMathLibrary::FindLookAtRotation(start, m_result.Location);
	}

	//투사체 생성
	/*ABullet* bullet = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), start, rot);
	if (bullet)
	{
		bullet->SpawnBulletFx(AIWeaponDataAsset->BulletTrailFXNiagara, rot.Vector(), owner);
		bullet->OnBulletHitDelegate.AddUObject(this, &UAIWeaponComponent::SpawnImpactEffect);
	}*/

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
}

