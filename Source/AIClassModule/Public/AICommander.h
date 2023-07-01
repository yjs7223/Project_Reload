// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AICommander.generated.h"

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
		TMap<int, AActor*> List_RDivision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TMap<int, ECombat> List_Combat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TMap<int, FVector> List_Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TMap<int, FVector> List_ChkLocation; //��ǥ �����̼��� �۾� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TMap<int, float> List_Suppression;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<AActor*> arrOutActors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		int AddIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool En_Start;//��ī���� ������ ��������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		EState state;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class AEncounterSpace* encounter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class ASubEncounterSpace* suben;


public:
};

