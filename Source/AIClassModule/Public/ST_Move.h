// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "ST_Move.generated.h"

USTRUCT(BlueprintType)
struct AICLASSMODULE_API FST_Move : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Move_ChangeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Move_ParallelTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Spd_Normal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Spd_Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Spd_Hit;
};
