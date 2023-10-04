// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"
#include "CameraControllComponent.generated.h"

USTRUCT(BlueprintType)
struct FCameraControllData
{
	GENERATED_BODY()
public:
	/** 카메라 위치 값 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector camerapos;
	/** 카메라 회전 값 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator camerarot;
	/** 스프링암 길이 값 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetArmLenght;
	/** 카메라 배율 값 (FOV) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float magnification = 1.0f;

	/** 카메라 위치 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PosSpeed;
	/** 카메라 회전 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotSpeed;
	/** 스프링암 길이 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraLengthSpeed;
	/** 카메라 배율 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float magnificationSpeed;

	void operator+=(const FCameraControllData& other);

};

UCLASS(Blueprintable)
class UCameraControllPakage : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Initalize();
	UFUNCTION(BlueprintImplementableEvent)
	bool IsControlled();

	void NativeInitalize(class ACharacter* _owner);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCameraControllData Data;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class ACharacter> owner;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraControllPakageDataAsset* m_CameraControllStructData;

	float InitArmLength;
	float InitFOV;


protected:
	ACharacter* owner;
	class UCameraComponent* m_FollowCamera;
	class USpringArmComponent* m_FollowSpringArm;
	class UCoverComponent* m_Cover;
	class UWeaponComponent* m_Weapon;
	class UBaseInputComponent* m_Input;
	class UBaseCharacterMovementComponent* m_Movement;
};

UCLASS()
class UCameraControllPakageDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	void foreach(TFunction<void(FString, UCameraControllPakage*)> fn);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, TSubclassOf<UCameraControllPakage>> CameraControllMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCameraControllPakage> DefaultCameraControll;
};
