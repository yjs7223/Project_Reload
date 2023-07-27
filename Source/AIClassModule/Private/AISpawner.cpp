// Fill out your copyright notice in the Description page of Project Settings.


#include "AISpawner.h"
#include "AICharacter.h"
#include "AI_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include "AICommander.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "EngineGlobals.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AAISpawner::AAISpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAISpawner::BeginPlay()
{
	Super::BeginPlay();

	curSpawnData = spawnData->FindRow<FST_Spawner>(*FString::FromInt(curWave), TEXT(""));
	commander = Cast<AAICommander>(UGameplayStatics::GetActorOfClass(GetWorld(), AAICommander::StaticClass()));
	player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	pointTime = 0;
	pointSpawnCheck = false;
}

// Called every frame
void AAISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WaveControl(DeltaTime);

	if (commander != nullptr)
	{
		if (player && check_Overlap)
		{
			//SpawnLastPoint(DeltaTime);
		}
	}
}

void AAISpawner::SpawnWave()
{
	// 기본 소환 위치
	int spawn_Pos = (*curSpawnData).spawn_Spot;
	// 라이플
	if (rifleCount < (*curSpawnData).spawn_Wave[Enemy_Name::RIFLE])
	{
		// 스폰 위치 검사 후 변경
		spawn_Pos = SetSpawnSpot(spawn_Pos);

		// 생성
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Spawn!"));
		APawn* temp = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), enemy_Rifle, BT_Rifle, spawn_Spots[spawn_Pos]->GetActorLocation());

		// 생성되면서 자신을 생성한 스포너를 저장하도록 함
		Cast<AAICharacter>(temp)->mySpawner = this;
		commander->ListAdd(Cast<AActor>(temp));
		rifleCount++;
	}
	else
	{
		// 스폰 완료
		spawnCheck = true;
		rifleCount = 0;
	}

	spawn_Timer = 0;
	

	// 댕댕이
	/*int dogCount = (*curSpawnData).spawn_Wave[Enemy_Name::DOG];
	if (dogCount > 0)
	{
		for (int i = 0; i < dogCount; i++)
		{
			APawn* temp = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), enemy_Rifle, BT_Dog, GetActorLocation());
		}
	}*/
}

void AAISpawner::WaveControl(float DeltaTime)
{
	/*if (check_Overlap && !spawnCheck)
	{
		spawn_Timer += DeltaTime;
		if (spawn_Timer >= (*curSpawnData).spawn_Delay)
		{
			SpawnWave();
			spawn_Timer = 0;
		}
	}*/

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
					// 다음 웨이브
					NextWave();
				}
				break;
			case Spawn_Type::SECONDS:
				spawn_Timer += DeltaTime;
				if (spawn_Timer >= (*curSpawnData).spawn_Condition)
				{
					// 다음 웨이브
					NextWave();
				}
				break;
			}
		}
	}
}

int AAISpawner::SetSpawnSpot(int p_Spawn_Pos)
{
	// 플레이어와 가깝지 않다면 소환
	if (spawn_Spots[(*curSpawnData).spawn_Spot]->GetDistanceTo(player) >= 50)
	{
		p_Spawn_Pos = (*curSpawnData).spawn_Spot;
	}
	else
	{
		for (int i = 0; i < spawn_Spots.Num(); i++)
		{
			// 플레이어 거리가 50보다 큰 스폰위치가
			if (spawn_Spots[i]->GetDistanceTo(player) > 50)
			{
				// 처음 들어온 거라면 일단 넣기
				if (p_Spawn_Pos == (*curSpawnData).spawn_Spot)
				{
					p_Spawn_Pos = i;
				}
				// 누가 더 작은 지 확인하고 작은 걸로 넣기
				if (spawn_Spots[p_Spawn_Pos]->GetDistanceTo(player) >
					spawn_Spots[i]->GetDistanceTo(player))
				{
					p_Spawn_Pos = i;
				}
			}
		}
	}

	return p_Spawn_Pos;
}

void AAISpawner::NextWave()
{
	// 다음 웨이브로 넘기기
	curSpawnData = spawnData->FindRow<FST_Spawner>(*FString::FromInt(++curWave), TEXT(""));
	count_Kill = 0;
	spawnCheck = false;
}

void AAISpawner::SpawnEnable(bool p_flag)
{
	check_Overlap = p_flag;
}

void AAISpawner::SpawnLastPoint(float DeltaTime)
{
	if (commander->GetBlackboardComponent())
	{


		if (commander->Cmd_SightOut)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Point!"));
			pointTime += DeltaTime;
			if (pointTime >= 1 && !pointSpawnCheck)
			{
				if (commander->GetBlackboardComponent()->GetValueAsObject("Cmd_Target") != nullptr)
				{
					FActorSpawnParameters SpawnParams;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

					// Spawn
					AActor* temp = GetWorld()->SpawnActor<AActor>(lastPoint, GetWorld()->GetFirstPlayerController()->GetPawn()->GetTransform(), SpawnParams);
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("LastPoint!"));
					pointSpawnCheck = true;
					pointTime = 0;


					commander->GetBlackboardComponent()->SetValueAsObject("Cmd_Target", temp);
				}
			}
		}
		else if (!commander->Cmd_SightOut)
		{

			pointTime = 0;
			pointSpawnCheck = false;
			if (commander->GetBlackboardComponent()->GetValueAsObject("Cmd_Target") != Cast<UObject>(player))
			{
				GetWorld()->DestroyActor(Cast<AActor>(commander->GetBlackboardComponent()->GetValueAsObject("Cmd_Target")));
			}
			commander->GetBlackboardComponent()->SetValueAsObject("Cmd_Target", Cast<UObject>(player));

			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("NoPoint!"));

		}
	}
}