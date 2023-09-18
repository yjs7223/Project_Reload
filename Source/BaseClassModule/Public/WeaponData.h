// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "WeaponData.generated.h"

/**
 * 
 */
UCLASS()
class BASECLASSMODULE_API UWeaponData : public UObject
{
	GENERATED_BODY()
	
};

USTRUCT(Atomic, BlueprintType)
struct FWeaponRowStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	//전체 탄약수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HoldAmmo_num;

	//데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Max_Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Min_Damage;

	//연사속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Fire_Rate;

	//최대사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Max_Range;

	//데미지 편차값(랜덤값)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Deviation;

	//발사횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Max_FireCount;

};
