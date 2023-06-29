// Fill out your copyright notice in the Description page of Project Settings.


#include "AISpawner.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>

// Sets default values
AAISpawner::AAISpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 소환 목록 삽입
	// 라이플
	static ConstructorHelpers::FObjectFinder<TSubclassOf<APawn>> RifleEnemy(TEXT("Blueprint'/Game/SGJ/BP_AICharacter.BP_AICharacter'"));
	if (RifleEnemy.Succeeded())
	{
		enemy_Rifle = RifleEnemy.Object;
	}
	// 자폭 댕댕이 (추가 예정)
	/*static ConstructorHelpers::FObjectFinder<AActor> DogEnemy(TEXT("댕댕이 래퍼런스 복붙"));
	if (DogEnemy.Succeeded())
	{
		enemy_Dog = DogEnemy;
	}*/
}

// Called when the game starts or when spawned
void AAISpawner::BeginPlay()
{
	Super::BeginPlay();
	
	curSpawnData = Cast<FST_Spawner>(spawnData);
}

// Called every frame
void AAISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAISpawner::SpawnWave()
{
	// BT 형변환
	UBehaviorTree* rifleBT = Cast<UBehaviorTree>(enemy_Rifle);
	//UBehaviorTree* dogBT = Cast<UBehaviorTree>(enemy_Dog);

	// 라이플 생성 수
	int rifleCount = curSpawnData->spawn_Wave[Enemy_Name::RIFLE];
	if (rifleCount > 0)
	{
		for (int i = 0; i < rifleCount; i++)
		{
			APawn* temp = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), enemy_Rifle, rifleBT, GetActorLocation());
		}
	}
	// 댕댕이 생성 수
	/*int dogCount = curSpawnData->spawn_Wave[Enemy_Name::DOG];
	if (dogCount > 0)
	{
		for (int i = 0; i < dogCount; i++)
		{
			APawn* temp = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), enemy_Rifle, dogBT, GetActorLocation());
		}
	}*/
}

void AAISpawner::WaveControl()
{

}

