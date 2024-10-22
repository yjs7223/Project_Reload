// Fill out your copyright notice in the Description page of Project Settings.


#include "EncounterSpace.h"
#include "SubEncounterSpace.h"
#include "AICommander.h"
#include "AICharacter.h"
#include "AI_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
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
	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AEncounterSpace::OnOverlapBegin);
	AI_Check = false;
	AI_ActiveFalse = false;
}

// Called when the game starts or when spawned
void AEncounterSpace::BeginPlay()
{
	Super::BeginPlay();
	commander = Cast<AAICommander>(UGameplayStatics::GetActorOfClass(GetWorld(), AAICommander::StaticClass()));
	AI_Check = false;
	AI_ActiveFalse = false;
	// add
	if (spawn != nullptr)
	{
		spawn->en = this;
	}
}

// Called every frame
void AEncounterSpace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AICheck();
	/*if (!AI_Check)
	{
		
	}*/
	if (AI_ActiveTrue)
	{
		for (auto m_ai : AIArray)
		{
			Cast<AAICharacter>(m_ai)->Init();
		}
		AI_ActiveTrue = false;
	}
}

void AEncounterSpace::AICheck()
{
	this->GetOverlappingActors(AIArray, AAICharacter::StaticClass());
	//AI_Check = true;
}

void AEncounterSpace::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherActor->ActorHasTag("Player"))
	{
		if (commander != nullptr)
		{
			if (commander->Now_en != this)
			{
				commander->m_en = this;
			}
			if (commander->Now_suben != suben)
			{
				commander->m_suben = suben;
			}
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("encounter"));
		}
	}
}

