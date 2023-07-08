// Fill out your copyright notice in the Description page of Project Settings.



#include "AICommander.h"
#include "AICharacter.h"
#include "AIStatComponent.h"
#include "EncounterSpace.h"
#include "SubEncounterSpace.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "ST_Suppression.h"
// Sets default values
AAICommander::AAICommander()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//encounter = CreateDefaultSubobject<AEncounterSpace>(TEXT("Encounter"));
	PrimaryActorTick.bCanEverTick = true;
	AddIndex = 0;
	MapList_Start = false;
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SuppressionDataObject(TEXT("DataTable'/Game/Aws/AI_Stat/DT_Suppression.DT_Suppression'"));
	if (DT_SuppressionDataObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
		DT_Suppression = DT_SuppressionDataObject.Object;
	}
	SetDataTable("Rifle_E");
	
}

// Called when the game starts or when spawned
void AAICommander::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEncounterSpace::StaticClass(), EncounterArray);
}

// Called every frame
void AAICommander::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	s_time += DeltaTime;
	ListSet();


}

void AAICommander::ListSet()
{
	for (auto& en : EncounterArray)
	{
		if (Cast<AEncounterSpace>(en)->LevelActive)
		{
			for (auto& sub : Cast<AEncounterSpace>(en)->LevelArray)
			{
				if (Cast<ASubEncounterSpace>(sub)->LevelActive)
				{
					if (!MapList_Start)
					{
						ListStartSet(Cast<ASubEncounterSpace>(sub));
					}
					else
					{
						ListTickSet(Cast<ASubEncounterSpace>(sub));
					}
				}
			}
		}
		
	}
}

void AAICommander::ListReset(ASubEncounterSpace* sub)
{
	List_Division.Reset();
	List_RDivision.Reset();
	List_Combat.Reset();
	List_Location.Reset();
	List_Suppression.Reset();
}

void AAICommander::ListStartSet(ASubEncounterSpace* sub)
{
	for (auto& subAi : Cast<ASubEncounterSpace>(sub)->AIArray)
	{
		List_Division.Add(subAi, AddIndex);
		List_RDivision.Add(AddIndex, subAi);
		List_Combat.Add(AddIndex, ECombat::Patrol);
		List_Location.Add(AddIndex, subAi->GetActorLocation());
		List_Suppression.Add(AddIndex, 0.0f);
		AddIndex++;
	}
	MapList_Start = true;
}

void AAICommander::ListTickSet(ASubEncounterSpace* sub)
{
	for (auto ai : Cast<ASubEncounterSpace>(sub)->AIArray)
	{
		auto FindActor = List_Division.Find(ai); 
		
		if (FindActor)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("AICOMMENDER"));
			List_Location.Add(*FindActor, ai->GetActorLocation());
			//List_Suppression.Add(*FindActor, Cast<AAICharacter>(actor)->FindComponentByClass<UAIStatComponent>()->sup_total);
			/*if (s_time >= sup_sharetime)
			{
				SuppressionShare();
				s_time = 0;
			}*/
		}
		else
		{
			if (List_Division.IsEmpty())
			{
				Cast<ASubEncounterSpace>(sub)->LevelActive = false;
			}
		}
	}
}

void AAICommander::SuppressionShare()
{
	List_Suppression.ValueSort([](float a, float b) {
		return a > b;
		});
	int key = List_Suppression[0];
	List_Location.GenerateValueArray(AILocation);
	FVector* MaxSupLoc = List_Location.Find(key);
	for (auto ai : AILocation)
	{
		if (ai != *MaxSupLoc)
		{
			List_Suppression.Add(*List_Location.FindKey(ai)) = 
				(*List_Suppression.Find(*List_Location.FindKey(ai)) / 5) * (1 - ((FVector::Distance(*MaxSupLoc, ai))/ sup_sharerange));
		}
	}
	
	
}

void AAICommander::SetDataTable(FName EnemyName)
{
	FST_Suppression* SuppressionData = DT_Suppression->FindRow<FST_Suppression>(EnemyName, FString(""));
	if (SuppressionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData Succeed!"));

		sup_sharerange = SuppressionData->Sup_ShareRange;
		sup_sharetime = SuppressionData->Sup_ShareTime;
	}
}



