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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommender)
		class UBoxComponent* CollisionMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommender)
		bool LevelActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommender)
		bool LevelOneActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommender)
		bool LevelTwoActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommender)
		bool LevelThreeActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommender)
		bool LevelFourActive;

public:
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
