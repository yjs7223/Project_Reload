// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ST_AIShot.generated.h"


USTRUCT(BlueprintType)
struct FST_AIShot : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIShotDataBase)
		float Shot_ShootDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIShotDataBase)
		float Shot_MaxRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIShotDataBase)
		float Shot_MinRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIShotDataBase)
		float Shot_MaxDmg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIShotDataBase)
		float Shot_MinDmg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIShotDataBase)
		float RecoilMax_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIShotDataBase)
		float RecoilMin_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIShotDataBase)
		float Recoil_Range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIShotDataBase)
		int Shot_MaxCount;


};

UCLASS()
class AICLASSMODULE_API UST_AIShot2 : public UObject
{
	GENERATED_BODY()
};