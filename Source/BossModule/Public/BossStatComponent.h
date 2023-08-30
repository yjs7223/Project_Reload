// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatComponent.h"
#include "BossStatComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BOSSMODULE_API UBossStatComponent : public UStatComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UBossStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*UFUNCTION(BlueprintCallable)
		void Attacked(float p_damage) override;
	void Attacked(float p_damage, FHitResult result) override;
	void Attacked(FHitResult result) override;*/

};
