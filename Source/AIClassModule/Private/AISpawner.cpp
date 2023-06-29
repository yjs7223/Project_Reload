// Fill out your copyright notice in the Description page of Project Settings.


#include "AISpawner.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>

// Sets default values
AAISpawner::AAISpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ��ȯ ��� ����
	// ������
	static ConstructorHelpers::FObjectFinder<TSubclassOf<APawn>> RifleEnemy(TEXT("Blueprint'/Game/SGJ/BP_AICharacter.BP_AICharacter'"));
	if (RifleEnemy.Succeeded())
	{
		enemy_Rifle = RifleEnemy.Object;
	}
	// ���� ����� (�߰� ����)
	/*static ConstructorHelpers::FObjectFinder<AActor> DogEnemy(TEXT("����� ���۷��� ����"));
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
	// BT ����ȯ
	UBehaviorTree* rifleBT = Cast<UBehaviorTree>(enemy_Rifle);
	//UBehaviorTree* dogBT = Cast<UBehaviorTree>(enemy_Dog);

	// ������ ���� ��
	int rifleCount = curSpawnData->spawn_Wave[Enemy_Name::RIFLE];
	if (rifleCount > 0)
	{
		for (int i = 0; i < rifleCount; i++)
		{
			APawn* temp = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), enemy_Rifle, rifleBT, GetActorLocation());
		}
	}
	// ����� ���� ��
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

