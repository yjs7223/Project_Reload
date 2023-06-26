// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseInputComponent.generated.h"


USTRUCT(BlueprintType)
struct FInputData
{
	GENERATED_USTRUCT_BODY()

	FVector movevec;
	uint8 IsRuning : 1;
	uint8 IsFire : 1;
	uint8 IsAiming : 1;
	uint8 IsReload : 1;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BASECLASSMODULE_API UBaseInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseInputComponent();

protected:
	virtual void BeginPlay() override;

public:
	FInputData* getInput();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Editor)
	ACharacter* owner;

	bool mCanUnCrouch;
	FInputData m_inputData;
};
