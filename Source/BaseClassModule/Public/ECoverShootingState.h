// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



UENUM(BlueprintType)
enum class ECoverShootingState : uint8
{
	None = 0	UMETA(Hidden),
	Front		UMETA(DisplayName = "Front"),
	Right		UMETA(DisplayName = "Right"),
	Left		UMETA(DisplayName = "Left"),
	MAX			UMETA(Hidden)
};

ENUM_CLASS_FLAGS(ECoverShootingState)
