// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "DroneAICharacter.generated.h"

UCLASS()
class DRONEMODULE_API ADroneAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADroneAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class UCharacterMovementComponent* CharMovement;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class UStaticMeshComponent* m_mesh;


	//MoveTimeLine
	class FOnTimelineFloat LerpDroneMoveToFunction; // (1)
	class FOnTimelineEvent LerpDroneMoveTimelineFinish; // (2)
	UFUNCTION()
	void LerpDroneMoveTo(float Value); // (3)
	UFUNCTION()
	void LerpDroneMoveFinish(); // (4)

	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UTimelineComponent* LerpDroneMoveToTimeline; // (5)
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* LerpDroneMoveToFloat; // (6)


	void MoveToTimelineSetting();

	UFUNCTION(BlueprintCallable, Category = "DroneMoveTo")
	void StartMoveTo();

	class AActor* m_player;

	class ADroneAIController* CTR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	bool IsMoving = false;
};
