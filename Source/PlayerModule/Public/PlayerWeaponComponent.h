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

	void RecoilTick(float p_deltatime);

	void StartRecoil();

	void StopRecoil();

	void StartRecovery();

	float EasingOut(float x);
	float easeOutExpo(float t, float b, float c, float d);
	//void StopRcovery();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class APlayerCharacter* owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_firerate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_firecount;
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

	float pp;
	float tt;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator PlayerStartRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator PlayerNowRot;*/


	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bRecovery;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float recoveryTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float yawRecoveryValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float pitchRecoveryValue;*/
};
