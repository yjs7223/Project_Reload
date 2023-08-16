// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponComponent.h"
#include "PlayerWeaponComponent.generated.h"

DECLARE_DELEGATE(FOnChangedCrossHairAmmoDelegate);
DECLARE_DELEGATE(FOnChangedCrossHairHitDelegate);
DECLARE_DELEGATE(FOnChangedCrossHairDieDelegate);
DECLARE_DELEGATE(FOnChangedAmmoUIDelegate);

DECLARE_DELEGATE(FOnPlayReloadUIDelegate);

DECLARE_DELEGATE(FOnVisibleCrossHairUIDelegate);
DECLARE_DELEGATE(FOnVisibleAmmoUIDelegate);

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UPlayerWeaponComponent : public UWeaponComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UPlayerWeaponComponent();

	FOnChangedCrossHairAmmoDelegate OnChangedCrossHairAmmoDelegate;
	FOnChangedCrossHairHitDelegate OnChangedCrossHairHitDelegate;
	FOnChangedCrossHairDieDelegate OnChangedCrossHairDieDelegate;
	FOnVisibleCrossHairUIDelegate OnVisibleCrossHairUIDelegate;


	FOnVisibleAmmoUIDelegate OnVisibleAmmoUIDelegate;
	FOnChangedAmmoUIDelegate OnChangedAmmoUIDelegate;

	FOnPlayReloadUIDelegate OnPlayReloadUIDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitData();

	UFUNCTION(BlueprintCallable)
		void Fire() override;
	UFUNCTION(BlueprintCallable)
	void StartAiming();
	UFUNCTION(BlueprintCallable)
	void StopAiming();
	UFUNCTION(BlueprintCallable)
	void StartFire();
	UFUNCTION(BlueprintCallable)
	void StopFire();


	UFUNCTION(BlueprintCallable)
	void StartReload();
	void StopReload();
	void WeaponMeshSetting(class UWeaponDataAsset* WeapondataAsset);


	void ReloadTick(float Deltatime);

	void RecoilTick(float p_deltatime);

	void StartRecoil();

	void StopRecoil();

	void RecoveryTick(float p_deltatime);

	void StartRecovery();

	void StopRcovery();

	float easeOutExpo(float t, float b, float c, float d);

	void SpawnDecal(FHitResult result);

	void PlayRandomShotSound();

	void PlayCameraShake(float scale);

	void SpawnImpactEffect(FHitResult result);

	void SpawnField(FHitResult result);

	void Threaten();

	float CalcDamage(FHitResult result, FVector2D p_damage);

	static bool CheckActorTag(AActor* actor, FName tag);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class APlayerCharacter* owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* PlayerWeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWeaponDataAsset* RifleDataAssets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWeaponDataAsset* PistolDataAssets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWeaponDataAsset* WeaponDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UHitImapactDataAsset* HitImpactDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMaterialInstance* Decal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FHitResult m_result;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool headhit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ammoinfinite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_firerate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_firecount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_spreadPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_turnValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_lookupValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_IturnValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_IlookupValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_dValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTimerHandle fHandle;

	FTimerHandle AimingTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bRecoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float recoilTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float yawRecoilValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float pitchRecoilValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D yawRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D pitchRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float recoilAlpha;

	float TickCount;
	float reloadCount;
	int reloadvalue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bRecovery;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RecoveryTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float yawRecoveryValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float pitchRecoveryValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator startRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator recoveryRot;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UGameplayStatics* GameStatic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystem* MuzzleFireParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystem* BulletTracerParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraComponent* shotFXComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* shotFXNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraComponent* hitFXComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* hitFXNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<USoundWave*> ShotSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UMatineeCameraShake> fireShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UBlueprint> fieldActor;

	float MaxRange;
	float Deviation;
};
