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

	// �ѱ� ȭ�� ��ƼŬ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		UParticleSystem* shotFX;

	// �ѱ� ��ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		FVector shotPos;


	// ���� �ݵ� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float recoil_Radius;

	// AI�� �ִ� �ݵ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float recoilMax_Radius;
	// AI�� �ּ� �ݵ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float recoilMin_Radius;

	// AI�� �ݵ� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float recoil_Range;

	// AI�� �� ���� Ƚ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		int shot_MaxCount;


	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		bool shot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float shotDamage;

	// Hit Result
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FHitResult m_result;

	// �� ���̾ư���
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
