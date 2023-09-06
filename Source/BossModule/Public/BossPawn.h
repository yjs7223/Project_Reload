// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BossPawn.generated.h"

UCLASS()
class BOSSMODULE_API ABossPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABossPawn();

	UFUNCTION(BlueprintCallable, Category = "Boss")
	float GetActorDegree(FVector p_PlayerLocation, FVector p_SelfLocation);

	UFUNCTION(BlueprintCallable, Category = "Boss")
	void MoveInCircle(FVector p_Center, float p_Radius, float p_Speed, float Angle);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
