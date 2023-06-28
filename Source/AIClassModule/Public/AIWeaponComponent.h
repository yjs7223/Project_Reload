// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponComponent.h"
#include "NiagaraComponent.h"
#include "AIWeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API UAIWeaponComponent : public UWeaponComponent
{
	GENERATED_BODY()
public :
	UAIWeaponComponent();

	// 총구 화염 파티클
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		UParticleSystem* shotFX;

	// 총구 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		FVector shotPos;


	// 현재 반동 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float recoil_Radius;

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

	// 빔 나이아가라
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		UNiagaraSystem* shotFXNiagara;
	UPROPERTY()
		UNiagaraComponent* shotFXComponent;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// AI Shot
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void ShotAI(FVector loc, FRotator rot);
};
