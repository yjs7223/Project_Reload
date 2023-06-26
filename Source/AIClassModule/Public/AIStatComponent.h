// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatComponent.h"
#include "AIStatComponent.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API UAIStatComponent : public UStatComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UAIStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
