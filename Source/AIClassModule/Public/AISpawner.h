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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		class UBlackboardComponent* BlackboardComponent;
	// 현재 웨이브
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int curWave;

	// 스폰 관련 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* spawnData;
	// 현재 데이터 테이블
	FST_Spawner* curSpawnData;

	// 스포너 활성화 상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		bool check_Overlap;

	// 플레이어가 사살한 적의 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		int count_Kill;

	// 웨이브 생성 후 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		float count_Seconds;

	// 스폰할 적
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		TSubclassOf<APawn> enemy_Rifle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		class UBehaviorTree* BT_Rifle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		TSubclassOf<APawn> enemy_Dog;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		class UBehaviorTree* BT_Dog;
	
	// 딜레이용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		float spawn_Timer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class AAI_Controller* AIController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class AAICommander* commander;

	// 이번 웨이브 소환 여부
	bool spawnCheck;

	// 이번 웨이브에 소환 한 수
	int rifleCount;
	int dogCount;

	// 스포너 스폰 지점들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		TArray<AActor*> spawn_Spots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> lastPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		float pointTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		bool pointSpawnCheck;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 웨이브 스폰
	void SpawnWave();
	
	// 웨이브 컨트롤
	void WaveControl(float DeltaTime);

	// 스폰 지점 검사 후 변경
	int SetSpawnSpot(int m_Spawn_Pos);

	// 다음 웨이브 이동 및 초기화
	void NextWave();

	// 스포너 활/비활성화
	UFUNCTION(BlueprintCallable, Category = "Spawner")
		void SpawnEnable(bool p_flag);
	UFUNCTION(BlueprintCallable, Category = "Spawner")
		void SpawnLastPoint(float DeltaTime);
};
