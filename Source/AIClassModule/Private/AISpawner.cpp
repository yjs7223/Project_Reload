// Fill out your copyright notice in the Description page of Project Settings.


#include "AISpawner.h"
#include "AICharacter.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>

// Sets default values
AAISpawner::AAISpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 데이터 테이블 삽입
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/SGJ/DT_Spawner.DT_Spawner'"));
	if (DataTable.Succeeded())
	{
		spawnData = DataTable.Object;
	}
}

// Called when the game starts or when spawned
void AAISpawner::BeginPlay()
{
	Super::BeginPlay();
	
	curSpawnData = spawnData->FindRow<FST_Spawner>(*FString::FromInt(curWave), TEXT(""));
}

// Called every frame
void AAISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (check_Overlap && !spawnCheck)
	{
		spawn_Timer += DeltaTime;
		if (spawn_Timer >= (*curSpawnData).spawn_Delay)
		{
			SpawnWave();
			spawn_Timer = 0;
		}
	}

	// 마지막 웨이브인지 확인
	if (spawnCheck)
	{
		if ((*curSpawnData).last_Spawn)
		{
			check_Overlap = false;
		}
		else
		{
			switch ((*curSpawnData).spawn_Type)
			{
			case Spawn_Type::KILL:
				if (count_Kill >= (*curSpawnData).spawn_Condition)
				{
					// 다음 웨이브로 넘기기
					curSpawnData = spawnData->FindRow<FST_Spawner>(*FString::FromInt(++curWave), TEXT(""));
					count_Kill = 0;
				}
				break;
			case Spawn_Type::SECONDS:
				spawn_Timer += DeltaTime;
				if (spawn_Timer >= (*curSpawnData).spawn_Condition)
				{
					// 다음 웨이브로 넘기기
					curSpawnData = spawnData->FindRow<FST_Spawner>(*FString::FromInt(++curWave), TEXT(""));
					spawn_Timer = 0;
				}
				break;
			}
		}
	}
}

void AAISpawner::SpawnWave()
{
	// 기본 소환 위치
	AActor* spawn_Pos = (*curSpawnData).spawn_Spot.Get();
	// 라이플
	if (rifleCount < (*curSpawnData).spawn_Wave[Enemy_Name::RIFLE])
	{
		// 플레이어와 가깝지 않다면 소환
		if ((*curSpawnData).spawn_Spot->GetDistanceTo(GetWorld()->GetFirstPlayerController()->GetPawn()) >= 50)
		{
			spawn_Pos = (*curSpawnData).spawn_Spot.Get();
		}
		else
		{
			for (int i = 0; i < spawn_Spots.Num(); i++)
			{
				// 플레이어 거리가 50보다 큰 스폰위치가
				if (spawn_Spots[i]->GetDistanceTo(GetWorld()->GetFirstPlayerController()->GetPawn()) > 50)
				{
					// 처음 들어온 거라면 일단 넣기
					if (spawn_Pos == (*curSpawnData).spawn_Spot.Get())
					{
						spawn_Pos = spawn_Spots[i];
					}
					// 누가 더 작은 지 확인하고 작은 걸로 넣기
					if (spawn_Pos->GetDistanceTo(GetWorld()->GetFirstPlayerController()->GetPawn()) >
						spawn_Spots[i]->GetDistanceTo(GetWorld()->GetFirstPlayerController()->GetPawn()))
					{
						spawn_Pos = spawn_Spots[i];
					}
				}
			}
		}

		APawn* temp = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), enemy_Rifle, BT_Rifle, spawn_Pos->GetActorLocation());
		// 생성되면서 자신을 생성한 스포너를 저장하도록 함
		Cast<AAICharacter>(temp)->mySpawner = this;
		rifleCount++;
	}
	else
	{
		// 스폰 완료
		spawnCheck = true;
	}

	spawn_Timer = 0;
	

	// 댕댕이
	/*int dogCount = curSpawnData->spawn_Wave[Enemy_Name::DOG];
	if (dogCount > 0)
	{
		for (int i = 0; i < dogCount; i++)
		{
			APawn* temp = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), enemy_Rifle, BT_Dog, GetActorLocation());
		}
	}*/
}

void AAISpawner::WaveControl()
{

}

