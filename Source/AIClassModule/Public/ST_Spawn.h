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
	RIFLE , DOG
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
	// 생성 조건 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Spawn_Type spawn_Type;

	// 스폰 액터 사이의 딜레이 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float spawn_Delay;

	// 각 웨이브마다 소환할 유닛들
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<Enemy_Name, int> spawn_Wave;

	// 스폰 조건
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float spawn_Condition;

	// 유닛이 생성될 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TWeakObjectPtr<AActor> spawn_Spot;

	// 마지막 웨이브인지 확인
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool last_Spawn;
};