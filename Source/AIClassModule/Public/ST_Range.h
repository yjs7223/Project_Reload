// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ST_Range.generated.h"

USTRUCT(BlueprintType)
struct FST_Range : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIRangeDataBase)
		float Sight_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIRangeDataBase)
		float Sight_Angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIRangeDataBase)
		float Sight_Age;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIRangeDataBase)
		float LoseSight_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIRangeDataBase)
		float Sense_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIRangeDataBase)
		float NCoverMax_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIRangeDataBase)
		float AimFwd_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIRangeDataBase)
		float AimFwd_Angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIRangeDataBase)
		float AimBwd_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIRangeDataBase)
		float AimBwd_Angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIRangeDataBase)
		float AimSide_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIRangeDataBase)
		float AimSide_Angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIRangeDataBase)
		float Sup_HitHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIRangeDataBase)
		float Sup_HitRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIRangeDataBase)
		float Move_DecRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIRangeDataBase)
		float Target_MinRange;
	
	
};


UCLASS()
class AICLASSMODULE_API UST_Range2 : public UObject
{
	GENERATED_BODY()
};