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
	MAX			UMETA(Hidden)
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASECLASSMODULE_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FShotting);
public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void SetAmmo(int p_ammo);

	UFUNCTION(BlueprintCallable)
		void ReloadAmmo();

		virtual void Fire();

		float getAimYaw();
		float getAimPitch();

		void AimSetting();
		UFUNCTION(BlueprintCallable)
		void SetHandleing(bool isFaceRight, bool isCoverUse = false);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		int holdAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		int curAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		int maxAmmo;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		bool isFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		bool isReload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		bool isHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		bool isAiming;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Weapon)
		bool m_CanShooting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		FVector2D damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		FVector2D H_damage;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		FRotator aimOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class USkeletalMeshComponent* WeaponMesh;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class TSubclassOf<UAnimInstance> RifleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class TSubclassOf<UAnimInstance> PistolAnimation;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class USkeletalMesh* RifleMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class USkeletalMesh* PistolMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class USkeletalMesh* ShotgunMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		EWeaponType weapontype;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Editor)
	FName Weapon_Handle_R_Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Editor)
	FName Weapon_Handle_L_Name;
};
