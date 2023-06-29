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

	// AI ĳ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		AAICharacter* owner;

	// �ѱ� ȭ�� ��ƼŬ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		UParticleSystem* shotFX;

	// ���� �ݵ� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float recoil_Radius;

	// �ִ� ��Ÿ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float shot_MaxRange;
	// �ּ� ��Ÿ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		float shot_MinRange;

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

	// �߻�ü ���̾ư���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotSetting")
		UNiagaraSystem* shotFXNiagara;
	UPROPERTY()
		UNiagaraComponent* shotFXComponent;

	// ���� �ݵ����� ������ ���̺�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* AIShotData;
	// ���� ���� �ݵ����� ������ ���̺�
	FST_AIShott* curAIShotData;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// AI Shot
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void ShotAI();
};
