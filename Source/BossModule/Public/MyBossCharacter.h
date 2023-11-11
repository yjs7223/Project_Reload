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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:

	UFUNCTION(BlueprintCallable, Category = "Boss")
	float GetActorDegree(FVector p_PlayerLocation, FVector p_SelfLocation);

	UFUNCTION(BlueprintCallable, Category = "Boss")
	void MoveInCircle(FVector Origin, float Radius, float Angle);

	UFUNCTION(BlueprintCallable, Category = "Boss")
	FVector GetLocationInCircle(FVector Origin, float Radius, float Angle);

};
