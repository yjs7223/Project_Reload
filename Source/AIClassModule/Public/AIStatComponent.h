// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatComponent.h"
#include "AIStatComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AICLASSMODULE_API UAIStatComponent : public UStatComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UAIStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
		void Attacked(float p_damage) override;
		void Attacked(float p_damage, FHitResult result) override;
		void Attacked(FHitResult result) override;
	UFUNCTION(BlueprintCallable)
		void SuppresionPoint();
	UFUNCTION()
		void SetDataTable(FName EnemyName);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		AActor* IndirectCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		AActor* AICommander;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		AActor* Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		class UDataTable* DT_Suppression;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float sup_total;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float sup_Input;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float sup_DecInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float sup_AimPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float sup_AimDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float AI_PlayerDis;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float sup_MaxRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float sup_Multi;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float Time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float sup_DelayTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float sup_MaxPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float sup_middlePoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		bool PlayerAtt_ai;


};
