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

	// ���� ���̺�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int curWave;

	// ���̺� ���� ������ ���̺�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* spawnData;
	// ���� ���̺� ���� ������ ���̺�
	FST_Spawner* curSpawnData;

	// ������ Ȱ��ȭ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		bool check_Overlap;

	// �÷��̾ ����� ���� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		int count_Kill;

	// �÷��̾ ����� ���� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSetting")
		float count_Seconds;

	// ��ȯ ������ ���
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

	// �̹� ���̺� �����ϱ�
	UFUNCTION(BlueprintCallable, Category = "SpawnerSetting")
		void SpawnWave();
	
	// ���� ��Ʈ��
	UFUNCTION(BlueprintCallable, Category = "SpawnerSetting")
		void WaveControl();
};
