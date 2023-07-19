// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraControllComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAYERMODULE_API UCameraControllComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCameraControllComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		bool isAimChangeFinish();

	ACharacter* owner;
	class UCameraComponent* m_FollowCamera;
	class USpringArmComponent* m_FollowSpringArm;
	class UCoverComponent* m_Cover;
	class UWeaponComponent* m_Weapon;
	class UBaseInputComponent* m_Input;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector camerapos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TMap<FString, FVector> camerplusvectors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector camerapos_Aiming;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector camerapos_FaceRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector camerapos_FaceLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector camerapos_Crouch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector camerapos_FrontRightPeek;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector camerapos_FrontLeftPeek;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector camerapos_HighRightPeek;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector camerapos_HighLeftPeek;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector camerapos_LowRightPeek;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector camerapos_LowLeftPeek;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FRotator camerarot_aiming;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FRotator camerarot_PeekingLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FRotator camerarot_PeekingLRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float AimingArmLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float IdleArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float TargetArmLenght;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float m_PosSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float m_RotSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float m_LengthSpeed;
};
