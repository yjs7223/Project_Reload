// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ST_Suppression.generated.h"


USTRUCT(BlueprintType)
struct FST_Suppression : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AISuppressionDataBase)
		float Sup_MaxRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AISuppressionDataBase)
		float Combat_Point_Idle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AISuppressionDataBase)
		float Combat_Point_Move;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AISuppressionDataBase)
		float Combat_Point_Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AISuppressionDataBase)
		float Combat_Point_Cover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AISuppressionDataBase)
		float Sup_DelayTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AISuppressionDataBase)
		float Sup_Multi;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AISuppressionDataBase)
		float Sup_LowInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AISuppressionDataBase)
		float Sup_DecPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AISuppressionDataBase)
		float Sup_DecTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AISuppressionDataBase)
		float Sup_ShareRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AISuppressionDataBase)
		float Sup_ShareTime;
};


UCLASS()
class AICLASSMODULE_API UST_Suppression2 : public UObject
{
	GENERATED_BODY()
};
