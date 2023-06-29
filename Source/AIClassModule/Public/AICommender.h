// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AICommender.generated.h"

UENUM(BlueprintType)
enum class ECombat : uint8
{
	Patrol 	UMETA(DisplayName = "Patrol"),
	Alaramed 	UMETA(DisplayName = "Alaramed"),
	MoveCover 	UMETA(DisplayName = "MoveCover"),
	Move 	UMETA(DisplayName = "Move"),
	Attack 	UMETA(DisplayName = "Attack"),
	InCover 	UMETA(DisplayName = "InCover")
};
UENUM(BlueprintType)
enum class EState : uint8
{
	Start 	UMETA(DisplayName = "Start"),
	Play 	UMETA(DisplayName = "Play"),
	End 	UMETA(DisplayName = "End")
};

UCLASS()
class AICLASSMODULE_API AAICommender : public AActor
{
	GENERATED_BODY()

		//using KEY = int32;

public:
	// Sets default values for this actor's properties
	AAICommender();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommender)
		TMap<AActor*, int> List_Division;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommender)
		TMap<int, AActor*> List_RDivision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommender)
		TMap<int, ECombat> List_Combat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommender)
		TMap<int, FVector> List_Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommender)
		TMap<int, FVector> List_ChkLocation; //��ǥ �����̼��� �۾� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommender)
		TMap<int, float> List_Suppression;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommender)
		TArray<AActor*> arrOutActors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommender)
		int AddIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommender)
		bool En_Start;//��ī���� ������ ��������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommender)
		EState state;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommender)
		class AEncounterSpace* encounter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommender)
		class ASubEncounterSpace* suben;


public:
	UFUNCTION()
		ASubEncounterSpace* LevelActiveCheck();
	UFUNCTION()
		void ListStartSet(ASubEncounterSpace* sub);
	UFUNCTION()
		void ListPlaySet(ASubEncounterSpace* sub);
	//UFUNCTION()
	//	void EnemyCheckPoint(AActor* otherActor);
};