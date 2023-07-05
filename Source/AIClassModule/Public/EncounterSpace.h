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
		class AAICommander* aic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class ASubEncounterSpace* subEn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class UBoxComponent* CollisionMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		bool LevelActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		TArray<class ASubEncounterSpace*> LevelArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		int LevelActiveNum;


public:
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		ASubEncounterSpace* LevelStartArrayActive();
	UFUNCTION()
		ASubEncounterSpace* LevelArrayActive();
	UFUNCTION()
		void LevelEndActive();
	UFUNCTION()
		void ListStartSet(ASubEncounterSpace* sub);
	UFUNCTION()
		void ListTickSet(ASubEncounterSpace* sub);
};
