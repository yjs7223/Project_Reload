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


	// 무기 스탯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		bool shot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float shotDamage;

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
<<<<<<< Updated upstream
	FST_AIShott* curAIShotData;
=======
	struct FST_AIShot* curAIShotData;
>>>>>>> Stashed changes

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// AI Shot
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void ShotAI();
};
