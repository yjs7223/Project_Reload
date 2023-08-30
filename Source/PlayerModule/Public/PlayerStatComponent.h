// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatComponent.h"
#include "PlayerStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangedHeathDelegate, float);
//DECLARE_DELEGATE_OneParam(FOnChangedHeathDelegate, float);
DECLARE_MULTICAST_DELEGATE(FOnVisibleHPUIDelegate);
DECLARE_DELEGATE(FOnVisibleAttackedUIDelegate);

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UPlayerStatComponent : public UStatComponent
{
	GENERATED_BODY()

public:
	UPlayerStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	
public:
	UFUNCTION(BlueprintCallable)
	void SetHP(float p_HP) override;

	UFUNCTION(BlueprintCallable)
	void RecoverHP(float p_HP);

	UFUNCTION(BlueprintCallable)
	void Attacked(class ABaseCharacter* attacker, float p_damage = 0, EHitType hittype = EHitType::Normal, FVector attackPoint = FVector::ZeroVector) override;

	/*UFUNCTION(BlueprintCallable)
	void Attacked(float p_damage, class ACharacter* character) override*/;

	void RecoverHP(float p_HP) override;
public:
	FOnChangedHeathDelegate OnChangedHealthDelegate;
	FOnVisibleHPUIDelegate OnVisibleHPUIDelegate;
	FOnVisibleAttackedUIDelegate OnVisibleAttackedUIDelegate;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UMatineeCameraShake> AttackedCameraShake;
	
};
