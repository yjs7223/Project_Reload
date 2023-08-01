// Fill out your copyright notice in the Description page of Project Settings.


#include "SubEncounterSpace.h"
#include "AICharacter.h"
#include "AISpawner.h"
#include "EncounterSpace.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

// Sets default values
ASubEncounterSpace::ASubEncounterSpace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("C Mesh"));
	RootComponent = CollisionMesh;

	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &ASubEncounterSpace::OnOverlapBegin);
	CollisionMesh->OnComponentEndOverlap.AddDynamic(this, &ASubEncounterSpace::OnOverlapEnd);
	
	LevelActive = false;
}

// Called when the game starts or when spawned
void ASubEncounterSpace::BeginPlay()
{
	Super::BeginPlay();

	// add
	if (spawn != nullptr)
	{
		spawn->suben = this;
	}
}

// Called every frame
void ASubEncounterSpace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (LevelActive)
	{
		EnemyAICheck();
		
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, CollisionMesh->GetScaledBoxExtent().ToString());
	}
}

void ASubEncounterSpace::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherActor->ActorHasTag("Player"))
	{
		LevelActive = true;
	}
}

void ASubEncounterSpace::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherActor->ActorHasTag("Player"))
	{
		LevelActive = false;
	}
}

void ASubEncounterSpace::EnemyAICheck()
{
	this->GetOverlappingActors(AIArray,AAICharacter::StaticClass());
	
}



