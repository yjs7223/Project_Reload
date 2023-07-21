// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatComponent.h"
#include "PlayerStatComponent.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UPlayerStatComponent : public UStatComponent
{
	GENERATED_BODY()
	
public:
	void Attacked(float p_damage) override;
};
