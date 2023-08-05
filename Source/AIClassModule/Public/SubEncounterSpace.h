// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SubEncounterSpace.generated.h"

UCLASS()
class AICLASSMODULE_API ASubEncounterSpace : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASubEncounterSpace();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class UBoxComponent* CollisionMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		int LevelNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool ArrayActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool LevelActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<AActor*> AIArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class AEncounterSpace* en;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class AAISpawner* spawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class AAICommander* commander;

public:
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void EnemyAICheck();

};
