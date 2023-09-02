// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EncounterSpace.generated.h"

UCLASS()
class AICLASSMODULE_API AEncounterSpace : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEncounterSpace();

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
		bool AI_Check;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool AI_ActiveTrue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool AI_ActiveFalse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class AAI_Controller* AIController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<AActor*> AIArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class ASubEncounterSpace* suben;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class AAICommander* commander;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class AAISpawner* spawn;

public:


	UFUNCTION()
		void AICheck();
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
