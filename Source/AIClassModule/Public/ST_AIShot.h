// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ST_AIShot.generated.h"

UCLASS()
class AICLASSMODULE_API UST_AIShot : public UObject
{
	GENERATED_BODY()
};

USTRUCT(Atomic, BlueprintType)
struct FST_AIShott : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	// 사격 딜레이
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float shot_ShootDelay;

	// 사격 최대 사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float shot_MaxRange;
	// 사격 최소 사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float shot_MinRange;

	// 사격 최대 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float shot_MaxDmg;
	// 사격 최소 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float shot_MinDmg;

	// 사격 최대 반동
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float recoilMax_Radius;
	// 사격 최소 반동
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float recoilMin_Radius;

	// 사격 범위의 원뿔 높이
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float recoil_Range;

	// 총 공격 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float shot_MaxCount;
};