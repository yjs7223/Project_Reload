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
		bool En_Check;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool ArrayActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool EncounterAIActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<AActor*> EncounterArray;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class AAICommander* commander;

public:
	UFUNCTION()
		void EncounterCheck();

};
