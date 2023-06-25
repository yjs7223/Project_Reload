// Fill out your copyright notice in the Description page of Project Settings.


#include "EncounterSpace.h"
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
	LevelOneActive = false;
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

}

void AEncounterSpace::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	LevelActive = true;
	LevelOneActive = true;
	
}

void AEncounterSpace::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	LevelActive = false;
}

