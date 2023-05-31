// Fill out your copyright notice in the Description page of Project Settings.


#include "AICommender.h"
#include "AICharacter.h"
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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAICharacter::StaticClass(), arrOutActors);
	for (auto& item : arrOutActors)
	{
		List_Division.Add(item, AddIndex);
		List_Combat.Add(AddIndex, ECombat::Patrol);
		List_Location.Add(AddIndex, item->GetActorLocation());
		List_Suppression.Add(AddIndex, 0.0);
		AddIndex++;
	}
}

// Called every frame
void AAICommender::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAICharacter::StaticClass(), arrOutActors);
	if (En_Start)
	{
		for (auto& item : arrOutActors)
		{
			List_Division.Add(item, AddIndex);
			List_Combat.Add(AddIndex, ECombat::Patrol);
			List_Location.Add(AddIndex, item->GetActorLocation());
			List_Suppression.Add(AddIndex, 0.0);
			AddIndex++;
		}
		En_Start = false;
	}
	for (auto& item : arrOutActors)
	{
		FindIndex = List_Division.Find(item);
		if (FindIndex != nullptr)
		{
			
		}
		else
		{
			if (List_Division.IsEmpty())
			{

			}
		}
	}
}

