// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.h"
#include "AICharacter.h"
#include "NiagaraComponent.h"
#include "ST_AIShot.h"
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

	// 현재 캐릭터 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Enemy_Name type;

	// 사격 대상
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* target;

	// AI 사격 상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		bool shot_State;

	// AI 사격 가능 상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		bool use_Shot_State;

	// AI 캐릭터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		AAICharacter* owner;

	// 총구 불꽃 파티클
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		UParticleSystem* shotFX;

	// 현재 사격 반동 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float recoil_Radius;

	// 최대 사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float shot_MaxRange;
	// 최소 사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float shot_MinRange;

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
		float shot_Delay;

	// 현재 딜레이 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float cur_Shot_Delay;

	// Hit Result
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FHitResult m_result;

	// 총알 효과 나이아가라
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		UNiagaraSystem* shotFXNiagara;
	UPROPERTY()
		UNiagaraComponent* shotFXComponent;

	// 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* AIShotData;
	// 현재 데이터 테이블
	struct FST_AIShot* curAIShotData;

	class AAICommander* commander;
	FHitResult result;

	// 총알 박히는 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraComponent* hitFXComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* hitFXNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UHitImapactDataAsset* HitImpactDataAsset;


	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	class UWeaponDataAsset* RifleDataAssets;
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// AI Shot
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void ShotAI();
	// AI Shot Timer
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void ShotAITimer(float t);
	// AI Shot Start
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void ShotAIStart();
	// AI Shot Stop
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void ShotAIStop();

	// AI Reload
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void ReloadAI();
	
	// AI Type Setting
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void AITypeSetting();

	// AI Sniper Check
	UFUNCTION(BlueprintCallable, Category = "Attack")
		bool AITypeSniperCheck();

	void CheckTrace();
	void AISpawnImpactEffect(FHitResult p_result);
};
