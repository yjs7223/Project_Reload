// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "DialogueScriptStruct.generated.h"

/**
 * 
 */
UCLASS()
class BASECLASSMODULE_API UDialogueScriptStruct : public UObject
{
	GENERATED_BODY()
	
};


USTRUCT(Atomic, BlueprintType)
struct FDialogueRowStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString dialogue_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString dialogue_Line;
};