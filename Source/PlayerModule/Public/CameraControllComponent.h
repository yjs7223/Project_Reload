// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"
#include "CameraControllComponent.generated.h"

namespace EEasingFunc {
	enum Type;
}
#pragma region FCameraControllDataElement
USTRUCT(BlueprintType)
struct FCameraControllDataElement 
{
	GENERATED_BODY()
public:
	void Initalize();
	virtual void Easing(float DeltaTime);

	float time;
	float InvblendIn;
	float InvblendOut;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float blendIn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float blendOut;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EEasingFunc::Type> posEaseType;


};
USTRUCT(BlueprintType)
struct FCameraControllDataElementReal : public FCameraControllDataElement
{
	GENERATED_BODY()
public:
	virtual void Easing(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetValue;
	float CurrentValue;
};

USTRUCT(BlueprintType)
struct FCameraControllDataElementVector : public FCameraControllDataElement 
{
	GENERATED_BODY()
public:
	virtual void Easing(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetValue;
	FVector CurrentValue;
};

USTRUCT(BlueprintType)
struct FCameraControllDataElementRotater : public FCameraControllDataElement
{
	GENERATED_BODY()
public:
	virtual void Easing(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator TargetValue;
	FRotator CurrentValue;
};
#pragma endregion

USTRUCT(BlueprintType)
struct FCameraControllData
{
	GENERATED_USTRUCT_BODY()
public:
	FCameraControllData();

	void Temp(float DeltaTime, bool isChecked);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCameraControllDataElementVector camerapos;
	/** 카메라 회전 값 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCameraControllDataElementRotater camerarot;
	/** 스프링암 길이 값 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCameraControllDataElementReal SpringArmLength;
	/** 카메라 배율 값 (FOV) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCameraControllDataElementReal magnification;

	float time;
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

	void NativeInitalize(class ACharacter* _m_PlayerCharacterr);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCameraControllData Data;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class ACharacter> m_PlayerCharacter;
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
	UFUNCTION(Exec)
	void SetDebugMode(bool isEnable);
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraControllPakageDataAsset* m_CameraControllStructData;

protected:
	TObjectPtr<ACharacter> m_PlayerCharacter;
	TObjectPtr<class UCameraComponent> m_FollowCamera;
	TObjectPtr<class USpringArmComponent> m_FollowSpringArm;
	TObjectPtr<class UCoverComponent> m_Cover;
	TObjectPtr<class UWeaponComponent> m_Weapon;
	TObjectPtr<class UBaseInputComponent> m_Input;
	TObjectPtr<class UBaseCharacterMovementComponent> m_Movement;


	FVector m_DefaultPos;
	FRotator m_DefaultRot;
	float m_DefaultArmLength;
	float m_DefaultMagnification;

	float InitFOV;

	bool DebugMode = false;
};

UCLASS()
class UCameraControllPakageDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	void foreach(TFunction<void(FString, UCameraControllPakage*)> fn);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataAsset")
	TMap<FString, TSubclassOf<UCameraControllPakage>> CameraControllMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (DisplayName = "FaceRight"))
	FCameraControllDataElement m_FaceRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (DisplayName = "Crouch"))
	FCameraControllDataElement m_Crouch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataBackUp", meta = (DisplayName = "Crouch"))
	FVector m_DefaultPos;
	FRotator m_DefaultRot;
	float m_DefaultArmLength;
	float m_DefaultMagnification;
};
