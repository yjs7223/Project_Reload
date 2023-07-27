// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ST_Commander.generated.h"

USTRUCT(BlueprintType)
struct FST_Commander : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommanderDataBase)
		float Siege_Range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommanderDataBase)
		float Detour_Range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommanderDataBase)
		float Detour_Angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommanderDataBase)
		float Ndetour_Angle;
};

UCLASS()
class AICLASSMODULE_API UST_Commander2 : public UObject
{
	GENERATED_BODY()
};
