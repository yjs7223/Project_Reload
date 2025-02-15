// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.h"
#include "AICharacter.h"
#include "NiagaraComponent.h"
#include "AIWeaponData.h"
#include "AIWeaponComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AICLASSMODULE_API UAIWeaponComponent : public UWeaponComponent
{
	GENERATED_BODY()
public :
	UAIWeaponComponent();

	// 사격 대상
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* target;


	// AI 사격 가능 상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		bool use_Shot_State;

	// AI 캐릭터
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		AAICharacter* owner;*/

	// 현재 사격 반동 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float recoil_Radius;

	// 최대 사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float shot_MaxRange;


	// AI의 최대 반동 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float recoilMax_Radius;
	// AI의 최소 반동 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float recoilMin_Radius;

	// AI의 반동 원뿔 높이
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float recoil_Range;

	// AI의 총 공격 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		int shot_MaxCount;

	// AI의 현재 공격 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		int cur_Shot_Count;

	// 최대 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float shot_MaxDmg;
	// 최소 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float shot_MinDmg;

	// 사격 딜레이
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float fire_Rate;

	// 현재 딜레이 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float cur_fire_Rate;

	// Hit Result
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FHitResult m_result;

	// 총알 효과 나이아가라
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		UNiagaraSystem* laserFXNiagara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		UNiagaraComponent* laserFXComponent;

	// 에이밍 효과 나이아가라
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		UNiagaraSystem* AimFlashFXNiagara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		UNiagaraComponent* AimFlashFXComponent;

	// 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* DT_AIWeaponData;
	// 현재 데이터 테이블
	struct FAIWeaponStruct* curAIWeaponData;

	class AAICommander* commander;
	FHitResult result;

	// 총알 박히는 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraComponent* hitFXComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* hitFXNiagara;


	// DA
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAIWeaponDataAsset* RifleDataAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAIWeaponDataAsset* SniperDataAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAIWeaponDataAsset* HeavyDataAsset;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMaterialInstance* Decal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystem* MuzzleFireParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystem* BulletTracerParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* AI_FireSound;
	//������ �븮�� �Ҹ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
		class USoundCue* ReloadCliplockedSound;

	//������ źâ ���ԼҸ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
		class USoundCue* ReloadMagInSound;

	//������ źâ ��ü�Ҹ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
		class USoundCue* ReloadMagOutSound;

	// Main DA
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAIWeaponDataAsset* AIWeaponDataAsset;

	class USkeletalMeshComponent* playerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		class ACharacter* player; //cast
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
		class USoundCue* FireSound;
	AActor* blackboardTarget;

	FTimerHandle timer;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, UStaticMeshComponent*> Attachments;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USpotLightComponent* SpotLightCmp;*/

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// AI Shot
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void Fire() override;
	// AI Shot Timer
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void ShotAITimer(const float t);
	// AI Shot Start
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void StartFire() override;
	// AI Shot Stop
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void StopFire() override;
	// AI Reload
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void ReloadAI();
	
	// AI Type Setting
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void InitData() override;
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void PlayRandomShotSound() override;

	// AI Sniper Check
	UFUNCTION(BlueprintCallable, Category = "Attack")
		bool AITypeSniperCheck();

	void CheckTrace();

	UFUNCTION(BlueprintCallable, Category = "Laser")
		void LaserOn();

	UFUNCTION(BlueprintCallable, Category = "Laser")
		void LaserOff();

	UFUNCTION(Category = "Debug", meta = (CallInEditor = "true"))
	//UFUNCTION(BlueprintCallable, Category = "AimFlash")
		void AimFalshOn();


	UFUNCTION(Category = "Debug", meta = (CallInEditor = "true"))
		//UFUNCTION(BlueprintCallable, Category = "AimFlash")
		void AimFalshOff();
};
