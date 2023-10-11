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
#include "EncounterSpace.h"
#include "AIZombie.h"
#include "WeaponComponent.h"

// Sets default values
AAISpawner::AAISpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerOn = false;
}

// Called when the game starts or when spawned
void AAISpawner::BeginPlay()
{
	Super::BeginPlay();
	curWave = 0;
	if (Cast<AAICommander>(UGameplayStatics::GetActorOfClass(GetWorld(), AAICommander::StaticClass())) != nullptr)
	{
		commander = Cast<AAICommander>(UGameplayStatics::GetActorOfClass(GetWorld(), AAICommander::StaticClass()));
	}

	player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	pointTime = 0;
	pointSpawnCheck = false;
	SetDataTable(curWave);
	Total_EnemyNum = (spawn_Wave[Enemy_Name::RIFLE] + spawn_Wave[Enemy_Name::SNIPER] + spawn_Wave[Enemy_Name::HEAVY], spawn_Wave[Enemy_Name::ZOMBIE]);
	TArray<AActor*> basechars;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), basechars);
	for (auto var : basechars)
	{
		if (UWeaponComponent::CheckActorTag(var, TEXT("Boss"))) {
			UWeaponComponent* weaponComp = var->FindComponentByClass<UWeaponComponent>();
			weaponComp->Dele_SpawnTrigger.BindUObject(this, &AAISpawner::TriggerOnTrue);
		}
	}
	/*for each (AActor * var in basechars)
	{
		if (UWeaponComponent::CheckActorTag(var, TEXT("Boss"))) {
			UWeaponComponent* weaponComp = var->FindComponentByClass<UWeaponComponent>();
			weaponComp->Dele_SpawnTrigger.BindUObject(this, &AAISpawner::TriggerOnTrue);
		}
	}*/
}

// Called every frame
void AAISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WaveControl(DeltaTime);

	if (commander != nullptr)
	{
		if (check_Overlap)
		{
			SpawnLastPoint(DeltaTime);
		}
	}
}

void AAISpawner::SpawnWave()
{
	// 라이플
	if (rifleCount < spawn_Wave[Enemy_Name::RIFLE])
	{
		// 스폰 위치 검사 후 변경
		spawn_Spot = SetSpawnSpot(spawn_Spot);

		// 생성
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Spawn!"));
		APawn* temp = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), enemy_Rifle, BT_Enemy, spawn_Spots[spawn_Spot]->GetActorLocation());

		AAICharacter* ai = Cast<AAICharacter>(temp);
		// 생성되면서 자신을 생성한 스포너를 저장하도록 함
		if (ai != nullptr)
		{
			ai->mySpawner = this;
			ai->Init();

			commander->ListAdd(Cast<AActor>(temp));
			rifleCount++;
			if (ai->GetController() != nullptr)
			{
				AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ai)->GetController());
				if (AIController != nullptr)
				{
					if (AIController->GetBlackboardComponent() != nullptr)
					{
						if (AIController->GetBlackboardComponent()->GetValueAsBool("Simple_Run") == false)
						{
							AIController->GetBlackboardComponent()->SetValueAsObject("Target", player);
							AIController->GetBlackboardComponent()->SetValueAsEnum("Combat", 1);
						}
					}
					
				}
			}
		}
	}
	else if (sniperCount < spawn_Wave[Enemy_Name::SNIPER])
	{
		// 스폰 위치 검사 후 변경
		spawn_Spot = SetSpawnSpot(spawn_Spot);

		// 생성
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Spawn!"));
		APawn* temp = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), enemy_Sniper, BT_Enemy, spawn_Spots[spawn_Spot]->GetActorLocation());

		AAICharacter* ai = Cast<AAICharacter>(temp);
		// 생성되면서 자신을 생성한 스포너를 저장하도록 함
		if (ai != nullptr)
		{
			ai->mySpawner = this;
			ai->Init();

			commander->ListAdd(Cast<AActor>(temp));
			sniperCount++;
			if (ai->GetController() != nullptr)
			{
				AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ai)->GetController());
				if (AIController != nullptr)
				{
					AIController->GetBlackboardComponent()->SetValueAsObject("Target", player);
				}
			}
		}
	}
	else if (heavyCount < spawn_Wave[Enemy_Name::HEAVY])
	{
		// 스폰 위치 검사 후 변경
		spawn_Spot = SetSpawnSpot(spawn_Spot);

		// 생성
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Spawn!"));
		APawn* temp = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), enemy_Heavy, BT_Enemy, spawn_Spots[spawn_Spot]->GetActorLocation());
		AAICharacter* ai = Cast<AAICharacter>(temp);
		// 생성되면서 자신을 생성한 스포너를 저장하도록 함
		if (ai != nullptr)
		{
			ai->mySpawner = this;
			ai->Init();
			commander->ListAdd(Cast<AActor>(temp));
			heavyCount++;
			if (ai->GetController() != nullptr)
			{
				AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ai)->GetController());
				if (AIController != nullptr)
				{
					AIController->GetBlackboardComponent()->SetValueAsObject("Target", player);
				}
			}
		}
	}
	else if (zombieCount < spawn_Wave[Enemy_Name::ZOMBIE])
	{
		// 스폰 위치 검사 후 변경
		spawn_Spot = SetSpawnSpot(spawn_Spot);

		// 생성
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Spawn!"));
		APawn* temp = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), enemy_Zombie, nullptr, GetActorLocation());
		AAIZombie* zombie = Cast<AAIZombie>(temp);

		// 생성되면서 자신을 생성한 스포너를 저장하도록 함
		if (zombie != nullptr)
		{
			zombie->mySpawner = this;
			zombie->target = spawn_Spots[0];
			//commander->ListAdd(Cast<AActor>(temp));
			zombieCount++;
		}
	}
	else
	{
		// 스폰 완료
		spawnCheck = true;
		rifleCount = 0;
		sniperCount = 0;
		heavyCount = 0;
		zombieCount = 0;
	}
}

void AAISpawner::WaveControl(const float DeltaTime)
{
	if (curSpawnData == nullptr || spawn_Spots.Num() <= 0)
	{
		return;
	}
	if (TriggerOn == true)
	{
		SetDataTable(curWave);
		if (!check_Overlap)
		{
			check_Overlap = true;
		}
		if (waveEnd)
		{
			waveEnd = false;
		}
		if (spawnCheck)
		{
			spawnCheck = false;
		}
		
	}
	// 마지막 웨이브인지 확인 및 스폰
	if (check_Overlap && !spawnCheck)
	{
		switch (spawn_Type)
		{
		case Spawn_Type::KILL:
			if (count_Kill >= spawn_Condition)
			{
				spawn_Delay += DeltaTime;
				if (spawn_Delay >= (*curSpawnData).spawn_Delay)
				{
					SpawnWave();
					spawn_Delay = 0;
				}
			}
			break;
		case Spawn_Type::SECONDS:
			spawn_Timer += DeltaTime;
			if (spawn_Timer >= spawn_Condition)
			{
				spawn_Delay += DeltaTime;
				if (spawn_Delay >= (*curSpawnData).spawn_Delay)
				{
					SpawnWave();
					spawn_Delay = 0;
					count_Kill = 0;
				}
			}
			break;
		case Spawn_Type::Trigger:
			if (TriggerOn)
			{
				spawn_Delay += DeltaTime;
				if (spawn_Delay >= (*curSpawnData).spawn_Delay)
				{
					SpawnWave();
					spawn_Delay = 0;
					count_Kill = 0;
				}
			}
		}
	}

	if (last_Spawn && spawnCheck)
	{
		TriggerOn = false;
		check_Overlap = false;
		waveEnd = true;
		curWave = 0;
		
	}
	// 다음 웨이브
	else if (spawnCheck)
	{
		NextWave();
	}
}

int AAISpawner::SetSpawnSpot(int p_Spawn_Pos)
{
	// 플레이어와 가깝지 않다면 소환w
	if (spawn_Spots[spawn_Spot]->GetDistanceTo(player) >= 0)
	{
		p_Spawn_Pos = spawn_Spot;
	}
	else
	{
		for (int i = 0; i < spawn_Spots.Num(); i++)
		{
			// 플레이어 거리가 50보다 큰 스폰위치가
			if (spawn_Spots[i]->GetDistanceTo(player) > 50)
			{
				// 처음 들어온 거라면 일단 넣기
				if (p_Spawn_Pos == spawn_Spot)
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
	count_Kill = 0;
	spawn_Timer = 0;
	// 다음 웨이브로 넘기기
	SetDataTable(++curWave);
	spawnCheck = false;
}

void AAISpawner::SpawnEnable(bool p_flag)
{
	check_Overlap = p_flag;
}

void AAISpawner::SpawnLastPoint(const float DeltaTime)
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
					cpyLastPoint = GetWorld()->SpawnActor<AActor>(lastPoint, player->GetTransform(), SpawnParams);
					//cpyLastPoint->SetActorLocation(cpyLastPoint->GetActorLocation() - FVector(0, 0, 30));

					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("LastPoint!"));
					pointSpawnCheck = true;
					pointTime = 0;
					if (en != nullptr)
					{
						for (auto& ai : en->AIArray)
						{
							AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ai)->GetController());
							if (AIController != nullptr)
							{
								AIController->GetBlackboardComponent()->SetValueAsObject("Target", cpyLastPoint);
							}
							else
							{
								//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("error : SpawnLastPoint() suben->AIArray -> AIController is nullptr"));
							}
						}
					}
				}
			}
		}
		else if (!commander->Cmd_SightOut)
		{
			pointTime = 0;
			pointSpawnCheck = false;
			if (cpyLastPoint != nullptr)
			{
				GetWorld()->DestroyActor(cpyLastPoint);
				commander->GetBlackboardComponent()->SetValueAsObject("Cmd_Target", player);
			}
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("NoPoint!"));
		}
	}
}

void AAISpawner::TriggerOnTrue()
{
	TriggerOn = true;
}

void AAISpawner::SetDataTable(int p_curWave)
{
	if (spawnData != nullptr)
	{
		curSpawnData = spawnData->FindRow<FST_Spawner>(*FString::FromInt(p_curWave), TEXT(""));
		if (curSpawnData != nullptr)
		{
			last_Spawn = curSpawnData->last_Spawn;
			spawn_Condition = curSpawnData->spawn_Condition;
			spawn_Delay = curSpawnData->spawn_Delay;
			spawn_Spot = curSpawnData->spawn_Spot;
			spawn_Type = curSpawnData->spawn_Type;
			spawn_Wave = curSpawnData->spawn_Wave;
		}
	}
}
