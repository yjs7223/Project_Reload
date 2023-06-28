// Fill out your copyright notice in the Description page of Project Settings.


#include "EncounterSpace.h"
#include "SubEncounterSpace.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
// Sets default values
AEncounterSpace::AEncounterSpace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("C Mesh"));
	RootComponent = CollisionMesh;

	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AEncounterSpace::OnOverlapBegin);
	CollisionMesh->OnComponentEndOverlap.AddDynamic(this, &AEncounterSpace::OnOverlapEnd);

	LevelActive = false;
}

// Called when the game starts or when spawned
void AEncounterSpace::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEncounterSpace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (auto& item : LevelArray)
	{
		if (item->LevelNum == LevelActiveNum)
		{
			if (item->LevelActive == false)
			{
				LevelActiveNum++;
				if (LevelArray.Num() < LevelActiveNum)
				{
					LevelActive = false;
				}
				for (auto& item2 : LevelArray)
				{
					if (item2->LevelNum == LevelActiveNum)
					{
						item2->LevelActive = true;
					}
				}
			}
		}
	}
}

void AEncounterSpace::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherActor->ActorHasTag("Player"))
	{
		LevelActive = true;
		LevelActiveNum = 1;
		for (auto& item : LevelArray)
		{
			if (item->LevelNum == 1)
			{
				item->LevelActive = true;
			}
		}
	}
	
}

void AEncounterSpace::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherActor->ActorHasTag("Player"))
	{
		LevelActive = false;
	}
}

