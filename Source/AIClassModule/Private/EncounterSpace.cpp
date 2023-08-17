// Fill out your copyright notice in the Description page of Project Settings.


#include "EncounterSpace.h"
#include "SubEncounterSpace.h"
#include "AICommander.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "CoverManager.h"

// Sets default values
AEncounterSpace::AEncounterSpace()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("C Mesh"));
	RootComponent = CollisionMesh;
	LevelActive = false;
	subencheck = false;
	ActiveStart = false;
	LevelActiveNum = 1;
	
}

// Called when the game starts or when spawned
void AEncounterSpace::BeginPlay()
{
	Super::BeginPlay();
	commander = Cast<AAICommander>(UGameplayStatics::GetActorOfClass(GetWorld(), AAICommander::StaticClass()));
	LevelActive = false;
	subencheck = false;
	ActiveStart = false;
	LevelActiveNum = 1;
}

// Called every frame
void AEncounterSpace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (LevelActive)
	{
		if (!subencheck)
		{
			SubEncounterCheck();
		}
		if (!LevelArray.IsEmpty())
		{
			for (auto sub : LevelArray)
			{
				Cast<ASubEncounterSpace>(sub)->en = this;
			}
		}
		
		LevelArrayActive();
		if (LevelArray.Num() <= 0)
		{
			LevelEndActive();
		}
	}
	else
	{
		LevelActiveNum = 1;
	}
}


void AEncounterSpace::LevelArrayActive()
{
	for (auto& sub : LevelArray)
	{
		suben = nullptr;
		suben = Cast<ASubEncounterSpace>(sub);
		if (!ActiveStart)
		{
			LevelActiveNum = 1;
			if (suben->LevelNum == LevelActiveNum)
			{
				suben->LevelActive = true;
				if (commander == nullptr)
				{
					continue;
				}
				commander->m_suben = suben;
				ActiveStart = true;

				//return Cast<ASubEncounterSpace>(sub);

				//first encounter begin
				


			}
		}
		else {
			if (suben->LevelNum == LevelActiveNum)
			{
				if (suben->LevelActive == false)
				{
					LevelActiveNum++;
					
					for (auto& sub2 : LevelArray)
					{
						suben2 = Cast<ASubEncounterSpace>(sub2);
						if (suben2->LevelNum == LevelActiveNum)
						{
							suben2->LevelActive = true;
							commander->m_suben = suben2;
							//return Cast<ASubEncounterSpace>(sub2);

							
							//next encounter begin


						}
					}
				}
				//else return Cast<ASubEncounterSpace>(sub);
			}
		}
		
	}
	//return nullptr;
}

void AEncounterSpace::LevelEndActive()
{
	for (auto& item : LevelArray)
	{
		Cast<ASubEncounterSpace>(item)->LevelActive = false;
	}
	LevelActive = false;
	ActiveStart = false;
	subencheck = false;
}

void AEncounterSpace::SubEncounterCheck()
{
	this->GetOverlappingActors(LevelArray, ASubEncounterSpace::StaticClass());
	subencheck = true;
}

