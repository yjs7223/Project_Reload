// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponComponent.h"
#include "Input.h"
#include "PlayerWeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UPlayerWeaponComponent : public UWeaponComponent, public IInput
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UPlayerWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void bindInput(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void Fire() override;

	void StartAiming();

	void StopAiming();

	void StartFire();

	void StopFire();

	void StartReload();

	UFUNCTION(BlueprintCallable)
		void AddRecoil();

	UFUNCTION(BlueprintCallable)
		void Recovery();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class APlayerCharacter* owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_firerate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_firecount;
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
		FVector2D m_recoilAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D m_recoveryAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bResetRecoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bUpdateRecoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bAddRecoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCurveVector* recoilcurve;






	FTimerHandle fHandle;
};
