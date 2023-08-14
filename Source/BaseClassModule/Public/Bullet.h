// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class BASECLASSMODULE_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnBulletFx(class UNiagaraSystem* BulletFXNiagara, const FVector& ShootDirection);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* CollisionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraComponent* BulletFXComponent;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	class UNiagaraSystem* BulletFXNiagara;
};
