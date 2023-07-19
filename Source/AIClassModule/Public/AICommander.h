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
//UENUM(BlueprintType)
//enum class EState : uint8
//{
//	Start 	UMETA(DisplayName = "Start"),
//	Play 	UMETA(DisplayName = "Play"),
//	End 	UMETA(DisplayName = "End")
//};

UCLASS()
class AICLASSMODULE_API AAICommander : public AAIController
{
	GENERATED_BODY()

		//using KEY = int32;

public:
	// Sets default values for this actor's properties
	AAICommander();
	virtual void OnPossess(APawn* pPawn) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void
		Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		UBlackboardComponent* BlackboardComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TMap<AActor*, int> List_Division;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TMap<int, AActor*> List_RDivision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TMap<int, ECombat> List_Combat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TMap<int, FVector> List_Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TMap<int, FVector> List_ChkLocation; 
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
		class AAICharacter* ACharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float s_time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float sup_sharerange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float sup_sharetime;
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
		TArray<FVector> CoverCompliteArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<FHitResult> results;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool enemycover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool Cmd_SightOut;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool SightIn_CHK;
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


};

