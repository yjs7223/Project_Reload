// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ST_AIBaseStat.generated.h"

USTRUCT(BlueprintType)
struct FST_AIBaseStat : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIBaseStatDataBase)
		float AI_HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIBaseStatDataBase)
		float AI_Speed_Move;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIBaseStatDataBase)
		float AI_Speed_ShotMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIBaseStatDataBase)
		float AI_Speed_CrouchMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIBaseStatDataBase)
		float AI_Def;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIBaseStatDataBase)
		float AI_HitReactionScale;

	
};

UCLASS()
class AICLASSMODULE_API UST_AIBaseStat2 : public UObject
{
	GENERATED_BODY()
};
