// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatComponent.h"
#include "ST_Spawn.h"
#include "AICharacter.h"
#include "AIStatComponent.generated.h"


DECLARE_DELEGATE(FOnVisibleEnemyHPUIDelegate);
DECLARE_DELEGATE(FOnChangeEnemyHPUIDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AICLASSMODULE_API UAIStatComponent : public UStatComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UAIStatComponent();
	FOnVisibleEnemyHPUIDelegate OnVisibleEnemyHPUIDelegate;
	FOnChangeEnemyHPUIDelegate OnChangeEnemyHPUIDelegate;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
		void IndirectAttacked(float p_Value) override;
	UFUNCTION(BlueprintCallable)
		void Attacked(float p_damage = 0, class ABaseCharacter* attacker = nullptr, EHitType hittype = EHitType::Normal, FVector attackPoint = FVector::ZeroVector) override;
	UFUNCTION(BlueprintCallable)
		void SuppresionPoint();
	UFUNCTION()
		void SetDataTable(const FName EnemyName);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		AActor* IndirectCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		AActor* AICommander;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class ACharacter* player; //cast
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		class UDataTable* DT_Suppression;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		class UDataTable* DT_AIBaseStat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		CombatState combat;
	// 자신의 병과
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Enemy_Name type;
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
		float sup_MinPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float sup_middlePoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float sup_DecPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float sup_DecTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float shot_MaxRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float shot_MinRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float shot_MaxDmg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float shot_MinDmg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float DI_SupRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		bool PlayerAtt_ai;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		float Def;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		class AAI_Controller* AIController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		class UAICharacterMoveComponent* moveoncmp;


};
