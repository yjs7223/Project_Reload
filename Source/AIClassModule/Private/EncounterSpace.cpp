// Fill out your copyright notice in the Description page of Project Settings.

#include "AICommender.h"
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
	if (LevelActive != false)
	{
		if (LevelArrayActive() != nullptr)
		{
			subEn = LevelArrayActive();
			ListTickSet(subEn);
		}
		
	}

}

void AEncounterSpace::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherActor->ActorHasTag("Player"))
	{
		subEn = LevelStartArrayActive();
		ListStartSet(subEn);
	}

}

void AEncounterSpace::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherActor->ActorHasTag("Player"))
	{
		LevelActive = false;
		LevelEndActive();
	}
}

ASubEncounterSpace* AEncounterSpace::LevelStartArrayActive()
{
	LevelActive = true;
	LevelActiveNum = 1;
	for (auto& item : LevelArray)
	{
		if (item->LevelNum == 1)
		{
			item->LevelActive = true;
			return item;
		}
	}
	return nullptr;
}

ASubEncounterSpace* AEncounterSpace::LevelArrayActive()
{
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
					return nullptr;
				}
				for (auto& item2 : LevelArray)
				{
					if (item2->LevelNum == LevelActiveNum)
					{
						item2->LevelActive = true;
						return item2;
					}
				}
			}
			else return item;
		}
	}
	return nullptr;
}

void AEncounterSpace::LevelEndActive()
{
	for (auto& item : LevelArray)
	{
		item->LevelActive = false;
	}
}

void AEncounterSpace::ListStartSet(ASubEncounterSpace* sub)
{
	for (auto& item : sub->AIArray)
	{
		aic->List_Division.Add(item, aic->AddIndex);
		aic->List_RDivision.Add(aic->AddIndex, item);
		aic->List_Combat.Add(aic->AddIndex, ECombat::Patrol);
		aic->List_Location.Add(aic->AddIndex, item->GetActorLocation());
		aic->List_Suppression.Add(aic->AddIndex, 0.0);
		aic->AddIndex++;
	}
}

void AEncounterSpace::ListTickSet(ASubEncounterSpace* sub)
{
	for (auto& item : sub->AIArray)
	{
		auto FindActor = aic->List_Division.Find(item); //적이 죽었을땐 ai캐릭터 자체에서 디비전 지우기
		if (FindActor != nullptr)
		{
			aic->List_Location.Add(*FindActor, sub->GetActorLocation());
			aic->List_Location.Add(*FindActor, sub->GetActorLocation());//목표 위치 설정 후 변수넣기
			aic->List_Suppression.Add(*FindActor, 0.0); //나중에 서프레션 변수 넣을 예정
		}
		else
		{
			if (aic->List_Division.IsEmpty())
			{
				sub->LevelActive = false;
			}
		}
	}
}