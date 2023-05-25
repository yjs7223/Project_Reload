// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Input.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInput : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BASECLASSMODULE_API IInput
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void bindInput(class UInputComponent* PlayerInputComponent);
};
