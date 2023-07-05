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

	LevelActive = false;
	LevelActiveNum = 1;
	
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
	if (LevelActive)
	{
		SubEncounterCheck();
		if (LevelArrayActive() != nullptr)
		{
			LevelArrayActive();
		}
		
	}

}


ASubEncounterSpace* AEncounterSpace::LevelArrayActive()
{
	for (auto& sub : LevelArray)
	{
		if (!ActiveStart)
		{
			LevelActiveNum = 1;
			if (Cast<ASubEncounterSpace>(sub)->LevelNum == LevelActiveNum)
			{
				Cast<ASubEncounterSpace>(sub)->LevelActive = true;
				ActiveStart = true;
				return Cast<ASubEncounterSpace>(sub);
			}
		}
		else {
			if (Cast<ASubEncounterSpace>(sub)->LevelNum == LevelActiveNum)
			{
				if (Cast<ASubEncounterSpace>(sub)->LevelActive == false)
				{
					LevelActiveNum++;
					if (LevelArray.Num() < LevelActiveNum)
					{
						LevelActive = false;
						LevelEndActive();
						return nullptr;
					}
					for (auto& sub2 : LevelArray)
					{
						if (Cast<ASubEncounterSpace>(sub2)->LevelNum == LevelActiveNum)
						{
							Cast<ASubEncounterSpace>(sub2)->LevelActive = true;
							return Cast<ASubEncounterSpace>(sub2);
						}
					}
				}
				else return Cast<ASubEncounterSpace>(sub);
			}
		}
		
	}
	return nullptr;
}

void AEncounterSpace::LevelEndActive()
{
	for (auto& item : LevelArray)
	{
		Cast<ASubEncounterSpace>(item)->LevelActive = false;
	}
}

void AEncounterSpace::SubEncounterCheck()
{
	this->GetOverlappingActors(LevelArray, ASubEncounterSpace::StaticClass());
}

//void AEncounterSpace::ListStartSet(ASubEncounterSpace* sub)
//{
//	for (auto& item : sub->AIArray)
//	{
//		aic->List_Division.Add(item, aic->AddIndex);
//		aic->List_RDivision.Add(aic->AddIndex, item);
//		aic->List_Combat.Add(aic->AddIndex, ECombat::Patrol);
//		aic->List_Location.Add(aic->AddIndex, item->GetActorLocation());
//		aic->List_Suppression.Add(aic->AddIndex, 0.0);
//		aic->AddIndex++;
//	}
//}

//void AEncounterSpace::ListTickSet(ASubEncounterSpace* sub)
//{
//	for (auto& item : sub->AIArray)
//	{
//		auto FindActor = aic->List_Division.Find(item); //���� �׾����� aiĳ���� ��ü���� ����� �����
//		if (FindActor != nullptr)
//		{
//			aic->List_Location.Add(*FindActor, sub->GetActorLocation());
//			aic->List_Location.Add(*FindActor, sub->GetActorLocation());//��ǥ ��ġ ���� �� �����ֱ�
//			aic->List_Suppression.Add(*FindActor, 0.0); //���߿� �������� ���� ���� ����
//		}
//		else
//		{
//			if (aic->List_Division.IsEmpty())
//			{
//				sub->LevelActive = false;
//			}
//		}
//	}
//}