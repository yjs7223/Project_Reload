// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

DECLARE_DELEGATE(FDele_SpawnTrigger);

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None = 0	UMETA(Hidden),
	Pistol	UMETA(DisplayName = "Pistol"),
	Rifle	UMETA(DisplayName = "Rifle"),
	Shotgun	UMETA(DisplayName = "Shotgun"),
	Heavy	UMETA(DisplayName = "Heavy"),
	MAX			UMETA(Hidden)
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASECLASSMODULE_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FPlayShottingAinmationDelegate);

public:
	FPlayShottingAinmationDelegate shootingAnimation;
	FDele_SpawnTrigger Dele_SpawnTrigger;
public:
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//������ �ʱ�ȭ
	virtual void InitData();

	//ź�� �ʱ⼳��
	UFUNCTION(BlueprintCallable)
	void SetAmmo(int p_ammo);
	void CalculateBlockingTick(float p_deltatime);

	virtual void StartFire();
	virtual void StopFire();
	virtual void Fire();
	virtual void StartAiming();
	virtual void StopAiming();

	bool CanReload();
	float getAimYaw();
	float getAimPitch();

	void AimSetting();

	UFUNCTION(BlueprintCallable)
	void SetHandleing(bool isFaceRight, bool isCoverUse = false);

	virtual void PlayRandomShotSound();

	void SpawnImpactEffect(FHitResult result);

	float CalcDamage(FHitResult result, FVector2D p_damage);

	static bool CheckActorTag(AActor* actor, FName tag);
	UFUNCTION(BlueprintCallable)
	bool IsWeaponBlocking();
	UFUNCTION(BlueprintCallable)
	FVector getWeaponHitLocation();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	class ABaseCharacter* owner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int holdAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int curAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int maxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	bool bFire;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	bool bReload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	bool bHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	bool bAiming;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Weapon)
	bool m_CanShooting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FVector2D damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_firerate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int m_firecount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Deviation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FRotator aimOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	class USkeletalMeshComponent* WeaponMesh;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	class TSubclassOf<UAnimInstance> RifleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	class TSubclassOf<UAnimInstance> PistolAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	EWeaponType weapontype;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UHitImapactDataAsset* HitImpactDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Editor)
	FName Weapon_Handle_R_Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Editor)
	FName Weapon_Handle_L_Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Editor)
	FName Arm_R_Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Editor)
	FName Arm_L_Name;
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Pakur"))
	TObjectPtr<class UActorComponent> m_PakurComp;

	bool m_IsWeaponBlocking;
	float m_WeaponDistance;
	TObjectPtr<class UCoverComponent> m_Cover;
	TObjectPtr<class UBaseInputComponent> m_Input;
	FVector m_WeaponHitLocation;

 };
