// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "MyBossCharacter.generated.h"

/**
 * 
 */
UCLASS()
class BOSSMODULE_API AMyBossCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Boss")
	float GetActorDegree(FVector p_PlayerLocation, FVector p_SelfLocation);

	UFUNCTION(BlueprintCallable, Category = "Boss")
	FVector MoveInCircle(FVector p_Center, float p_Radius, float Angle);

};
