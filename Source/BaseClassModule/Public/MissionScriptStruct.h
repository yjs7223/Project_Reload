// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MissionScriptStruct.generated.h"

/**
 * 
 */
UCLASS()
class BASECLASSMODULE_API UMissionScriptStruct : public UObject
{
	GENERATED_BODY()
	
};

USTRUCT(Atomic, BlueprintType)
struct FMissionRowStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString mission_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString mission_Desc;

};