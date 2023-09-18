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
	//��ü ź���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 HoldAmmo_num;

	//������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Max_Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Min_Damage;

	//����ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Fire_Rate;
	
	//�ִ��Ÿ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Max_Range;
	
	//������ ������(������)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Deviation;

	//�߻�Ƚ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Max_FireCount;
};
