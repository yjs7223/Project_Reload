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
	void SetHP(float p_HP) override;
	void Attacked(float p_damage) override;

	UFUNCTION(BlueprintCallable)
	void Attacked(float p_damage, class ACharacter* character) override;

public:
	FOnChangedHeathDelegate OnChangedHealthDelegate;
	FOnVisibleHPUIDelegate OnVisibleHPUIDelegate;
	FOnVisibleAttackedUIDelegate OnVisibleAttackedUIDelegate;
public:
	ACharacter* TargetEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UMatineeCameraShake> AttackedCameraShake;
	
};
