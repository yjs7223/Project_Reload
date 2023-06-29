// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ST_Spawn.generated.h"

UENUM(BlueprintType)
enum class Spawn_Type : uint8
{
	KILL, SECONDS
};

UENUM(BlueprintType)
enum class Enemy_Name : uint8
{
	RIFLE = 1, DOG
};

UCLASS()
class AICLASSMODULE_API UST_Spawn : public UObject
{
	GENERATED_BODY()
};

USTRUCT(Atomic, BlueprintType)
struct FST_Spawner : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	// ������ ���̺� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Spawn_Type spawn_Type;

	// ���� ���� ������ ������ �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float spawn_Delay;

	// �� ���̺긶�� ��ȯ�� ���ֵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<Enemy_Name, int> spawn_Wave;

	// ������ �ϱ� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float spawn_Condition;

	// ������ ������ ��ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> spawn_Spot;
};