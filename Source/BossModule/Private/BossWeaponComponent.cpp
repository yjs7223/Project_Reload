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

FRotator UBossWeaponComponent::Fire(FVector Start)
{
	FVector loc;
	FRotator rot;
	owner->GetController()->GetPlayerViewPoint(loc, rot);

	float x = 0, y = 0;

	x = FMath::RandRange(-recoil_Radius, recoil_Radius);
	y = FMath::RandRange(-recoil_Radius, recoil_Radius);

	//���� �߰��ؼ� �̸� �����ϱ�
	//FVector start = WeaponMesh->GetSocketLocation(TEXT("SM_MachineGun_L"));
	FVector playerLocation = playerMesh->GetSocketLocation(TEXT("spine_05"));

	FVector end = Start + ((playerLocation - Start).Rotation() + FRotator(x, y, 0)).Vector() * shot_MaxRange;

	FCollisionQueryParams traceParams;

	// ���� ���� üũ
	if (GetWorld()->LineTraceSingleByChannel(m_result, Start, playerLocation, ECC_Visibility, traceParams))
	{
		rot = UKismetMathLibrary::FindLookAtRotation(Start, m_result.Location);
		// AI�� ���� ���� ���� �� ��� �Ұ���
		if (m_result.GetActor()->ActorHasTag("Enemy"))
		{
			return rot;
		}
	}

	// ��� ���� üũ
	if (GetWorld()->LineTraceSingleByChannel(m_result, Start, end, ECC_GameTraceChannel6, traceParams))
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
		rot = UKismetMathLibrary::FindLookAtRotation(Start, m_result.Location);
	}

	//ABullet* bullet = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), Start, rot);
	/*if (bullet)
	{
		bullet->SpawnBulletFx(AIWeaponDataAsset->BulletTrailFXNiagara, rot.Vector(), owner);
		bullet->OnBulletHitDelegate.AddUObject(this, &UAIWeaponComponent::SpawnImpactEffect);
	}*/

	// ���� �ݵ��� �پ��
	if (recoil_Radius > recoilMin_Radius)
	{
		recoil_Radius -= (recoilMax_Radius - recoilMin_Radius) / shot_MaxCount;
	}
	else
	{
		// �ּҷ� ����
		recoil_Radius = recoilMin_Radius;
	}

	return rot;
}

