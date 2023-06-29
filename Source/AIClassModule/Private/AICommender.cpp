// Fill out your copyright notice in the Description page of Project Settings.



#include "AICommender.h"
#include "AICharacter.h"
#include "EncounterSpace.h"
#include "SubEncounterSpace.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AAICommender::AAICommender()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AddIndex = 0;

}

// Called when the game starts or when spawned
void AAICommender::BeginPlay()
{
	Super::BeginPlay();
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAICharacter::StaticClass(), arrOutActors);
	/*for (auto& item : arrOutActors)
	{
		List_Division.Add(item, AddIndex);
		List_RDivision.Add(AddIndex, item);
		List_Combat.Add(AddIndex, ECombat::Patrol);
		List_Location.Add(AddIndex, item->GetActorLocation());
		List_Suppression.Add(AddIndex, 0.0);
		AddIndex++;
	}*/
}

// Called every frame
void AAICommender::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	switch (state)
	{
	case EState::Start:
		if (LevelActiveCheck() != nullptr)
		{
			ListStartSet(LevelActiveCheck());
		}
		state = EState::Play;
		break;
	case EState::Play:
		if (LevelActiveCheck() != nullptr)
		{
			ListPlaySet(LevelActiveCheck());
		}
		else state = EState::Start;
		break;
	case EState::End:

		break;

	}
	
}

ASubEncounterSpace* AAICommender::LevelActiveCheck()
{
	if (encounter->LevelActive == true)
	{
		for (auto& item : encounter->LevelArray)
		{
			if (item->LevelActive == true)
			{
				return item;
			}
		}
	}
	return nullptr;
}

void AAICommender::ListStartSet(ASubEncounterSpace* sub)
{
	for (auto& item : sub->AIArray)

	{
		List_Division.Add(item, AddIndex);
		List_RDivision.Add(AddIndex, item);
		List_Combat.Add(AddIndex, ECombat::Patrol);
		List_Location.Add(AddIndex, item->GetActorLocation());
		List_Suppression.Add(AddIndex, 0.0);
		AddIndex++;
	}
}

void AAICommender::ListPlaySet(ASubEncounterSpace* sub)
{
	for (auto& item : sub->AIArray)
	{
		auto FindActor = List_Division.Find(item); //���� �׾����� aiĳ���� ��ü���� ����� �����
		if (FindActor != nullptr)
		{
			List_Location.Add(*FindActor, sub->GetActorLocation());
			List_Location.Add(*FindActor, sub->GetActorLocation());//��ǥ ��ġ ���� �� �����ֱ�
			List_Suppression.Add(*FindActor, 0.0); //���߿� �������� ���� ���� ����
		}
		else
		{
			if (List_Division.IsEmpty())
			{
				state = EState::End;
			}
		}
	}
}

