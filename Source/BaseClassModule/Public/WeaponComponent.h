// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None = 0	UMETA(Hidden),
	TE_Pistol	UMETA(DisplayName = "Pistol"),
	TE_Rifle	UMETA(DisplayName = "Rifle"),
	TE_Shotgun	UMETA(DisplayName = "Shotgun"),
	TE_Heavy	UMETA(DisplayName = "Heavy"),
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
public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//데이터 초기화
	virtual void InitData();

	//탄약 초기설정
	UFUNCTION(BlueprintCallable)
		void SetAmmo(int p_ammo);

		virtual void StartFire();

		virtual void StopFire();

		virtual void Fire();

		float getAimYaw();
		float getAimPitch();

		void AimSetting();
		UFUNCTION(BlueprintCallable)
		void SetHandleing(bool isFaceRight, bool isCoverUse = false);

		//virtual void WeaponMeshSetting(class UWeaponDataAsset* WeapondataAsset);

		void PlayRandomShotSound();

		void SpawnImpactEffect(FHitResult result);

		float CalcDamage(FHitResult result, FVector2D p_damage);

		static bool CheckActorTag(AActor* actor, FName tag);

public:
	//오너캐릭터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class ABaseCharacter* owner;

	//잔여탄약
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		int holdAmmo;

	//현재탄약
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		int curAmmo;

	//최대 장탄수
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
		class UBaseWeaponDataAsset* WeaponDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* WeaponDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UHitImapactDataAsset* HitImpactDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Editor)
	FName Weapon_Handle_R_Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Editor)
	FName Weapon_Handle_L_Name;
};
