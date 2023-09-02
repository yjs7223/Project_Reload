// Fill out your copyright notice in the Description page of Project Settings.


#include "SubEncounterSpace.h"
#include "AICharacter.h"
#include "AISpawner.h"
#include "AI_Controller.h"
#include "EncounterSpace.h"
#include "Components/BoxComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/Engine.h"
#include "AICommander.h"
#include "CoverManager.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ASubEncounterSpace::ASubEncounterSpace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("C Mesh"));
	RootComponent = CollisionMesh;

	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &ASubEncounterSpace::OnOverlapBegin);
	
	EncounterAIActive = false;
}

// Called when the game starts or when spawned
void ASubEncounterSpace::BeginPlay()
{
	Super::BeginPlay();
	commander = Cast<AAICommander>(UGameplayStatics::GetActorOfClass(GetWorld(), AAICommander::StaticClass()));
	En_Check = false;
	
}

// Called every frame
void ASubEncounterSpace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!En_Check)
	{
		EncounterCheck();
	}
	if (EncounterAIActive)
	{
		for (auto m_en : EncounterArray)
		{
			Cast<AEncounterSpace>(m_en)->AI_ActiveTrue = true;
		}
		EncounterAIActive = false;
	}

}

void ASubEncounterSpace::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherActor->ActorHasTag("Player"))
	{
		if (commander != nullptr)
		{
			if (commander->Now_suben != this)
			{
				commander->m_suben = this;
			}
		}
	}
}


void ASubEncounterSpace::EncounterCheck()
{
	this->GetOverlappingActors(EncounterArray,AEncounterSpace::StaticClass());
	En_Check = true;
}



