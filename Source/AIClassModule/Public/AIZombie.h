// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIZombie.generated.h"

UCLASS()
class AICLASSMODULE_API AAIZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ACharacter* player;

	class UCapsuleComponent* collisionMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AAISpawner* mySpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* target;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "CustomEvents")
		void BulletHit(FHitResult result);
};
