// Fill out your copyright notice in the Description page of Project Settings.


#include "EncounterTrigger.h"
#include "EncounterSpace.h"
#include "SubEncounterSpace.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "Kismet/KismetStringLibrary.h"

// Sets default values
AEncounterTrigger::AEncounterTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//en = CreateDefaultSubobject<AEncounterSpace>(TEXT("EncounterSpace"));
	CollisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("C Mesh"));
	RootComponent = CollisionMesh;

	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AEncounterTrigger::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AEncounterTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEncounterTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEncounterTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherActor->ActorHasTag("Player"))
	{
		en->LevelActive = true;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("encounter"));
		
	}
}

