// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "CameraControllComponent.generated.h"

UENUM(BlueprintType)
enum class ECameraState : uint8
{
	None = 0	UMETA(Hidden),
	Default		UMETA(DisplayName = "Default"),
	Vehicle		UMETA(DisplayName = "Vehicle"),
	MAX			UMETA(Hidden)
};



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
	class UBaseCharacterMovementComponent* m_Movement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDataTable* m_CameraControllData;
	struct FCameraControllData* m_Data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECameraState cameraState;
};

USTRUCT(Atomic, BlueprintType)
struct FCameraControllData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector camerapos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector camerapos_Runing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector camerapos_Cover;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector camerapos_Aiming;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector camerapos_FaceRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector camerapos_FaceLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector camerapos_Crouch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector camerapos_FrontRightPeek;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector camerapos_FrontLeftPeek;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector camerapos_HighRightPeek;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector camerapos_HighLeftPeek;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector camerapos_LowRightPeek;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector camerapos_LowLeftPeek;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator camerarot_aiming;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator camerarot_PeekingLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator camerarot_PeekingLRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AimingArmLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float IdleArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TargetArmLenght;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_PosSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_RotSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_LengthSpeed;
};
