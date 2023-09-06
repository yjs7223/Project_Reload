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

DECLARE_DELEGATE_TwoParams(FOnSpawnDamageUIDelegate, float, FHitResult);

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
	FOnSpawnDamageUIDelegate OnSpawnDamageUIDelegate;

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

	UFUNCTION(BlueprintCallable)
	void InitData() override;

	UFUNCTION(BlueprintCallable)
	void Fire() override;
	UFUNCTION(BlueprintCallable)
	void StartAiming();
	UFUNCTION(BlueprintCallable)
	void StopAiming();
	UFUNCTION(BlueprintCallable)
	void StartFire() override;
	UFUNCTION(BlueprintCallable)
	void StopFire() override;


	UFUNCTION(BlueprintCallable)
	void StartReload();
	void StopReload();
	UFUNCTION(BlueprintCallable)
	void WeaponChange();
	void WeaponMeshSetting(class UPlayerWeaponDataAsset* WeapondataAsset);


	void ReloadTick(float Deltatime);

	void RecoilTick(float p_deltatime);

	void StartRecoil();

	void StopRecoil();

	void RecoveryTick(float p_deltatime);

	void StartRecovery();

	void StopRcovery();

	float easeOutExpo(float t, float b, float c, float d);

	void SpawnDecal(FHitResult result);

	void PlayRandomShotSound() override;

	void PlayCameraShake(float scale);

	void SpawnField(FHitResult result);

	void Threaten();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* PlayerWeaponDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPlayerWeaponDataAsset* PlayerWeaponDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FHitResult m_result;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float head_mag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool headhit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ammoinfinite;

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
		TSubclassOf<class UMatineeCameraShake> fireShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UBlueprint> fieldActor;
};
