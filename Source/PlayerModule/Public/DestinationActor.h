// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestinationActor.generated.h"


DECLARE_DELEGATE_OneParam(FOnUpdateDesUIDelegate, AActor*)


UCLASS()
class PLAYERMODULE_API ADestinationActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestinationActor();

	FOnUpdateDesUIDelegate OnUpdateDesUIDelegate;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* BoxCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 desIndex;
};
