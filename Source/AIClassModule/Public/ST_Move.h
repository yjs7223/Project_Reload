// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ST_Move.generated.h"

USTRUCT(BlueprintType)
struct FST_Move : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMoveDataBase)
		float Move_ChangeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMoveDataBase)
		float Move_ParallelTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMoveDataBase)
		float Spd_Normal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMoveDataBase)
		float Spd_Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMoveDataBase)
		float Spd_Hit;
};

UCLASS()
class AICLASSMODULE_API UST_Move2: public UObject
{
	GENERATED_BODY()
};