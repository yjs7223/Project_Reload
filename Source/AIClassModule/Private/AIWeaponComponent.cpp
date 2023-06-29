// Fill out your copyright notice in the Description page of Project Settings.


#include "AIWeaponComponent.h"
#include "AICharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "StatComponent.h"
#include "ST_AIShot.h"
#include <Kismet/GameplayStatics.h>

UAIWeaponComponent::UAIWeaponComponent()
{
	// ������ ���̺� ����
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/Aws/AI_Stat/DT_AiShot.DT_AiShot'"));
	if (DataTable.Succeeded())
	{
		AIShotData = DataTable.Object;
	}

	// �ѱ� �Ҳ� ��ƼŬ ����
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ShotFX(TEXT("ParticleSystem'/Game/ThirdPersonKit/Particles/P_RealAssaultRifle_MF.P_AssaultRifle_MF'"));
	if (DataTable.Succeeded())
	{
		shotFX = ShotFX.Object;
	}

	// �Ѿ� ���̾ư��� ����
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ShotFXNiagara(TEXT("NiagaraSystem'/Game/SGJ/NS_BulletProjectile.NS_BulletProjectile'"));
	if (DataTable.Succeeded())
	{
		shotFXNiagara = ShotFXNiagara.Object;
	}
	
}

void UAIWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner<AAICharacter>();

	// ���� �����Ϳ� �ֱ� (�ϴ� �����÷� ����)
	curAIShotData = AIShotData->FindRow<FST_AIShott>(TEXT("Rifle"), TEXT(""));

	// ��� ���� ���� �Է�
	recoil_Range = curAIShotData->recoil_Range;
	recoilMax_Radius = curAIShotData->recoilMax_Radius;
	recoilMin_Radius = curAIShotData->recoilMin_Radius;
}


// Called every frame
void UAIWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAIWeaponComponent::ShotAI()
{
	//owner->bUseControllerRotationYaw = true;
	
	FVector loc;
	FRotator rot;
	owner->Controller->GetPlayerViewPoint(loc, rot);

	float x = 0, y = 0;

	x = FMath::RandRange(-recoil_Radius, recoil_Radius);
	y = FMath::RandRange(-recoil_Radius, recoil_Radius);

	FVector start = WeaponMesh->GetSocketLocation(TEXT("MuzzleFlashSocket"));
	FVector end = start + ((rot + FRotator(x, y, 0)).Vector() * 5000);
	FVector end2 = (rot + FRotator(x, y, 0)).Vector() * recoil_Range;
	FCollisionQueryParams traceParams;
	
	if (GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_Visibility, traceParams))
	{
		if (m_result.GetActor()->ActorHasTag("Player"))
		{
			//bb->SetValueAsVector(name, hitr.Location);
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Name : bbb")));
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Name : %s"), *hit.GetComponent()->GetName()));

			auto temp = m_result.GetActor()->FindComponentByClass<UStatComponent>();
			if (temp) {
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("actor1 : %s"), *temp->GetName()));
				temp->Attacked(shotDamage);
			}
			else
			{
				if (recoil_Radius >= recoilMin_Radius)
				{
					recoil_Radius -= recoilMax_Radius - recoilMin_Radius / shot_MaxCount;

					shot_MaxCount++;
				}
			}

		}

		shot = true;
	}

	//DrawDebugLine(GetWorld(), start, end, FColor::Orange, false, 0.1f);

	// �ѱ�ȭ�� ����Ʈ ����
	UGameplayStatics::SpawnEmitterAtLocation(this, shotFX, start, rot, true);

	// �߻�ü ���̾ư��� ����Ʈ ����
	shotFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, shotFXNiagara, start, rot + FRotator(x, y, 0));

	shotFXComponent->SetNiagaraVariableVec3("BeamEnd", end2);

	DrawDebugLine(GetWorld(), start, end, FColor::Orange, false, 0.1f);
	//name = "AttackLocation";
}
