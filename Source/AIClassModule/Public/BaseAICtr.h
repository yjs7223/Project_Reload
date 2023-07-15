// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAICtr.generated.h"




UCLASS()
class AICLASSMODULE_API ABaseAICtr : public AAIController
{
	GENERATED_BODY()

public :
	ABaseAICtr();

	static const FName Key_Cover;
	static const FName Key_CoverLocation;


protected:

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* pPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation()const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		class UDataTable* DT_Range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class UBehaviorTreeComponent* behavior_tree_component;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class UBehaviorTree* btree;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		float m_SightRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		float m_SightAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		float m_SightAge;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		float m_SenseRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		float m_SenseAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		float m_SenseAge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		float m_CoverMaxRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		float m_NCoverMaxRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		float m_AimFwdRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		float m_AimFwdAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		float m_AimBwdRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		float m_AimBwdAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		float m_AimSideRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		float m_AimSideAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		float m_SupHitHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		float m_SupHitRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		float m_MoveDecRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		class UAISenseConfig_Sight* SightConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		bool bIsPlayerDetected = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		float DistanceToPlayer = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		class ABaseCharacter* m_character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaesAICtr)
		class UBehaviorTree* BTAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaesAICtr)
		class UBlackboardData* BBAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaesAICtr)
		UBlackboardComponent* BlackboardComponent;
public:
	UFUNCTION(BlueprintCallable)
		void SetEnemy(FName EnemyName);
	UFUNCTION()
		void OnPawnDetected(const TArray<AActor*>& DetectedPawns);

};
