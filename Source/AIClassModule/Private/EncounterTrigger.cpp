// Fill out your copyright notice in the Description page of Project Settings.


#include "EncounterTrigger.h"
#include "EncounterSpace.h"
#include "SubEncounterSpace.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "Kismet/KismetStringLibrary.h"
#include "AICommander.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

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
	commander = Cast<AAICommander>(UGameplayStatics::GetActorOfClass(GetWorld(), AAICommander::StaticClass()));
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
		if (commander != nullptr)
		{
			en->LevelActive = true;
			for (auto encoun : commander->EncounterArray)
			{
				AEncounterSpace* encounter = Cast<AEncounterSpace>(encoun);
				if (encounter != en)
				{
					encounter->LevelActive = false;
					encounter->LevelEndActive();
				}
			}
			commander->m_en = en;
			commander->En_AIActive = true;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("encounter"));
		}
		
		
	}
}

