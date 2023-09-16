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
	// TMAP_LIST
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TMap<AActor*, int> List_Division;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		//TMap<int, ECombat> List_Combat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TMap<int, FVector> List_Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TMap<int, FVector> List_CoverPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TMap<int, float> List_Suppression;

	// TArray
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<AActor*> EncounterArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<float> Sup_Array;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<FVector> CoverArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<FVector> CoverEnArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<FVector> CoverEnemyArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<FVector> SiegeCoverArray; //포위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<FVector> DetourCoverArray; //우회

	//Class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class UDataTable* DT_Commander;
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
		class ACharacter* player; //cast
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class ASubEncounterSpace* m_suben;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class ASubEncounterSpace* Now_suben;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class AEncounterSpace* m_en;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class AEncounterSpace* Now_en;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float env_range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float siege_range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float detour_range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float detour_angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float ndetour_angle;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		int AddIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool MapList_Start;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float s_time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		FVector MaxSupLoc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float sup_value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool sightin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		int subenNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		FHitResult result;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool enemycover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool Cmd_SightOut;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool SightIn_CHK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		FVector nomalcover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TSubclassOf<class AActor> RedBallBlueprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class AActor* redball;

	class ACoverManager* CoverManager;
	class UAudioComponent* audiocomp;
public:
	UFUNCTION()
		void ListSet();
	UFUNCTION()
		void ListVoidReset();
	UFUNCTION()
		void ListAdd(AActor* ac);
	UFUNCTION()
		void ListStartSet(const AEncounterSpace* en);
	UFUNCTION()
		void ListTickSet();
	UFUNCTION()
		void SuppressionShare();

	UFUNCTION()
		void SetCommanderDataTable(const FName EnemyName);
	UFUNCTION()
		void TargetTickSet();
	UFUNCTION()
		void CoverPointEn(const AEncounterSpace* en);
	UFUNCTION()
		void CoverPointEnemy();
	UFUNCTION()
		void SiegeCoverPoint();
	UFUNCTION()
		void DetourCoverPoint();
	UFUNCTION()
		bool IsPlayerInsideFanArea(FVector CoverPoint, float LocationRadius, float FanAngle, FVector FanDirection); //위치, 범위, 각도, 방향

	UFUNCTION()
		bool IsCoverInsideFanArea(FVector CoverPoint, float FanAngle, FVector FanDirection);
	UFUNCTION(BlueprintCallable, Category = "AICommander")
		FVector OptimumPoint(const FVector FinalLocation, const AActor* AIactor, const FVector MiddleLocation);
	UFUNCTION(BlueprintCallable, Category = "AICommander")
		bool SameDetourPoint(const FVector FinalLocation, const FVector MiddleLocation);
	UFUNCTION(BlueprintCallable, Category = "AICommander")
		void ZeroListCoverPoint(const AActor* AIActor);
	UFUNCTION(BlueprintImplementableEvent)
		void RestoreArr();

};

