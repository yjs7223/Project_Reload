// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASECLASSMODULE_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

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

	// (void Fire() override)
	//UFUNCTION(BlueprintCallable)
		virtual void Fire();

public:
	UPROPERTY(BlueprintReadWrite, Category = Weapon)
		int maxAmmo;

	UPROPERTY(BlueprintReadWrite, Category = Weapon)
		int curAmmo;
		
	UPROPERTY(BlueprintReadWrite, Category = Weapon)
		bool isFire;

	UPROPERTY(BlueprintReadWrite, Category = Weapon)
		bool isReload;

	UPROPERTY(BlueprintReadWrite, Category = Weapon)
		bool isHit;

	UPROPERTY(BlueprintReadWrite, Category = Weapon)
		bool isAim;

	UPROPERTY(BlueprintReadWrite, Category = Weapon)
		class USkeletalMeshComponent* WeaponMesh;
};
