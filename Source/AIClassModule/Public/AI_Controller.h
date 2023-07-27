// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AI_Controller.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API AAI_Controller : public AAIController
{
	GENERATED_BODY()
public:
	AAI_Controller();

protected: 
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation()const override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		class UBlackboardData* BBAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		class UDataTable* DT_Range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		class UBehaviorTree* btree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		class UBehaviorTreeComponent* behavior_tree_component;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		class UAISenseConfig_Sight* SightConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		class UAISenseConfig_Hearing* HearingConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		bool bIsPlayerDetected = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		class AAICommander* commander;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class AAI_Controller* AIController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		float DistanceToPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		class ACharacter* player; //cast

public:
	UFUNCTION()//BlueprintCallable
		void SetEnemy(FName EnemyName);
	UFUNCTION()
		void OnTargetDetected(AActor* actor, FAIStimulus const Stimulus);
	UFUNCTION()
		void SetUseCover();

	UFUNCTION()
		void RunBTT();
	
};
