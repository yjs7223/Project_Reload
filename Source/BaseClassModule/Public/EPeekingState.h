// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



UENUM(BlueprintType)
enum class EPeekingState : uint8
{
	None = 0			UMETA(Hidden),
	FrontRight			UMETA(DisplayName = "FrontRight"),
	FrontLeft			UMETA(DisplayName = "FrontLeft"),
	HighRight			UMETA(DisplayName = "HighRight"),
	HighLeft			UMETA(DisplayName = "HighLeft"),
	LowRight			UMETA(DisplayName = "LowRight"),
	LowLeft				UMETA(DisplayName = "LowLeft"),
	MAX					UMETA(Hidden)
};

ENUM_CLASS_FLAGS(EPeekingState)



/*
Front = 0b00000001	UMETA(DisplayName = "Front"),
	High =	0b00000010	UMETA(DisplayName = "High"),
	Low =	0b00000100	UMETA(DisplayName = "Low"),
	Right = 0b00001000	UMETA(DisplayName = "Right"),
	Left =	0b00010000	UMETA(DisplayName = "Left"),
	Start = 0b00000000	UMETA(DisplayName = "Start"),
	End =	0b10000000	UMETA(DisplayName = "End"),

	FrontRightStart =	Front | Right | Start	UMETA(DisplayName = "FrontRightStart"),
	FrontRightEnd =		Front | Right | End		UMETA(DisplayName = "FrontRightEnd"),
	FrontLeftStart =	Front | Left | Start	UMETA(DisplayName = "FrontLeftStart"),
	FrontLeftEnd =		Front | Left | End		UMETA(DisplayName = "FrontLeftEnd"),
	HighRightStart =	High | Right | Start	UMETA(DisplayName = "HighRightStart"),
	HighRightEnd =		High | Right | End		UMETA(DisplayName = "HighRightEnd"),
	HighLeftStart =		High | Left | Start		UMETA(DisplayName = "HighLeftStart"),
	HighLeftEnd =		High | Left | End		UMETA(DisplayName = "HighLeftEnd"),
	LowRightStart =		Low | Right | Start		UMETA(DisplayName = "LowRightStart"),
	LowRightEnd =		Low | Right | End		UMETA(DisplayName = "LowRightEnd"),
	LowLeftStart =		Low | Left | Start		UMETA(DisplayName = "LowLeftStart"),
	LowLeftEnd =		Low | Left | End		UMETA(DisplayName = "LowLeftEnd"),
*/
