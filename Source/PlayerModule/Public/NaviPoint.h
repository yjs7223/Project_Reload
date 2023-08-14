// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NaviPoint.generated.h"

UCLASS()
class PLAYERMODULE_API ANaviPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANaviPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void NearDestroy();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* spherecomp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 NaviIndex;

};
