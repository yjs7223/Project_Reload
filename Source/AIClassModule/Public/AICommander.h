// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AICommander.generated.h"

UENUM(BlueprintType)
enum class ECombat : uint8
{
	Patrol 	UMETA(DisplayName = "Patrol"),
	MoveCover 	UMETA(DisplayName = "MoveCover"),
	Move 	UMETA(DisplayName = "Move"),
	Attack 	UMETA(DisplayName = "Attack"),
	InCover 	UMETA(DisplayName = "InCover")
};

UENUM(BlueprintType)
enum class ETarget_State : uint8
{
	Idle 	UMETA(DisplayName = "Idle"),
	Attack 	UMETA(DisplayName = "Attack"),
	Cover 	UMETA(DisplayName = "Cover"),
	Move 	UMETA(DisplayName = "Move")
};

UCLASS()
class AICLASSMODULE_API AAICommander : public AAIController
{
	GENERATED_BODY()

		//using KEY = int32;

public:
	// Sets default values for this actor's properties
	AAICommander();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void
		Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TMap<AActor*, int> List_Division;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TMap<int, ECombat> List_Combat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TMap<int, FVector> List_Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TMap<int, FVector> List_ChkLocation; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TMap<int, FVector> List_CoverPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TMap<int, float> List_Suppression;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<AActor*> EncounterArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<float> Sup_Array;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		int AddIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool MapList_Start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		AActor* actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class UDataTable* DT_Suppression;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class UBehaviorTreeComponent* behavior_tree_component;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class UBehaviorTree* btree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class UBlackboardData* BB_AICommander;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class AAIController* BaseAI_Ctr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class AAI_Controller* AIController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float s_time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float sup_sharerange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float sup_sharetime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float env_range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float Siege_angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float detour_radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float detour_angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float ndetour_angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		FVector MaxSupLoc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float sup_value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		UObject* Target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool sightin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool subenbool;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		int subenNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<FVector> CoverArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<FVector> CoverSubEnArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<FVector> CoverEnemyArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<FVector> SiegeCoverArray; //포위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<FVector> DetourCoverArray; //우회
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		FHitResult result;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool enemycover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool Cmd_SightOut;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool Patrol_CHK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool SightIn_CHK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		FVector nomalcover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class ACharacter* player; //cast


public:
	UFUNCTION()
		void ListSet();
	UFUNCTION()
		void ListReset(ASubEncounterSpace* sub);
	UFUNCTION()
		void ListStartSet(ASubEncounterSpace* sub);
	UFUNCTION()
		void ListTickSet(ASubEncounterSpace* sub, AEncounterSpace* en);
	UFUNCTION()
		void SuppressionShare(ASubEncounterSpace* sub);
	UFUNCTION()
		void SetDataTable(FName EnemyName);
	UFUNCTION()
		void TargetTickSet(ASubEncounterSpace* sub);
	UFUNCTION()
		void CoverPointSubEn(ASubEncounterSpace* sub);
	UFUNCTION()
		void CoverPointEnemy();
	UFUNCTION()
		void SiegeCoverPoint();
	UFUNCTION()
		void DetourCoverPoint();
	UFUNCTION()
		bool IsPlayerInsideFanArea(FVector CoverPoint, float LocationRadius, float FanAngle, FVector FanDirection); //위치, 범위, 각도, 방향

};

