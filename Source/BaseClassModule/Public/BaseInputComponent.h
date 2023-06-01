// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseInputComponent.generated.h"

UENUM(BlueprintType)
enum class EInputType : uint8
{
	None = 0	UMETA(Hidden),
	Input		UMETA(DisplayName = "Input"),
	Setting		UMETA(DisplayName = "Setting"),
	MAX			UMETA(Hidden)
};

ENUM_CLASS_FLAGS(EInputType)



USTRUCT(BlueprintType)
struct FInputData
{
	GENERATED_USTRUCT_BODY()
	DECLARE_DELEGATE(CoverInputEvent);

	FVector movevec;
	uint8 IsRuning	: 1;
	uint8 IsFire	: 1;
	uint8 IsAiming	: 1;
	uint8 IsReload	: 1;
};


UCLASS()
class BASECLASSMODULE_API UBaseInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseInputComponent();

protected:
	
	virtual void BeginPlay() override;


public:

	void SetInputPtr(TSharedPtr<FInputData> input_ptr);

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Runing(); 
	void Crouching();
	void StartFire();
	void StopFire();
	void StartAiming(); 
	void StopAiming();
	void StartReload();
	void CoverInputEvent();
	void StartPeeking();
	void StopPeeking();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Editor)
	EInputType m_inputType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Editor)
	ACharacter* owner;

	bool mCanUnCrouch;
	TSharedPtr<FInputData> m_inputData;
};
