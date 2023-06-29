// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ST_Spawn.h"
#include "AISpawner.generated.h"

UCLASS()
class AICLASSMODULE_API AAISpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAISpawner();

	// 현재 웨이브
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int curWave;

	// 웨이브 관련 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* spawnData;
	// 현재 웨이브 관련 데이터 테이블
	FST_Spawner* curSpawnData;

	// 스포너 활성화 상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		bool check_Overlap;

	// 플레이어가 사살한 적의 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		int count_Kill;

	// 플레이어가 사살한 적의 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		float count_Seconds;

	// 소환 가능한 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		TSubclassOf<APawn> enemy_Rifle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		TSubclassOf<APawn> enemy_Dog;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 이번 웨이브 스폰하기
	UFUNCTION(BlueprintCallable, Category = "SpawnerSetting")
		void SpawnWave();
	
	// 조건 컨트롤
	UFUNCTION(BlueprintCallable, Category = "SpawnerSetting")
		void WaveControl();
};
