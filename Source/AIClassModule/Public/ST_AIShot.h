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
	// ��� �����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float shot_ShootDelay;

	// ��� �ִ� ��Ÿ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float shot_MaxRange;
	// ��� �ּ� ��Ÿ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float shot_MinRange;

	// ��� �ִ� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float shot_MaxDmg;
	// ��� �ּ� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float shot_MinDmg;

	// ��� �ִ� �ݵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float recoilMax_Radius;
	// ��� �ּ� �ݵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float recoilMin_Radius;

	// ��� ������ ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float recoil_Range;

	// �� ���� Ƚ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float shot_MaxCount;
};