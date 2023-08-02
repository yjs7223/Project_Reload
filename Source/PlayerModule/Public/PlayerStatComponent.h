// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatComponent.h"
#include "PlayerStatComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnChangedHeathDelegate, float);
DECLARE_DELEGATE(FOnVisibleHPUIDelegate);
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
	void Attacked(float p_damage, class ABaseCharacter* character) override;

public:
	FOnChangedHeathDelegate OnChangedHealthDelegate;
	FOnVisibleHPUIDelegate OnVisibleHPUIDelegate;
	FOnVisibleAttackedUIDelegate OnVisibleAttackedUIDelegate;
public:
	ABaseCharacter* TargetEnemy;
	
};
