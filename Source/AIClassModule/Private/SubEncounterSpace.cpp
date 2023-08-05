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
	
	LevelActive = false;
}

// Called when the game starts or when spawned
void ASubEncounterSpace::BeginPlay()
{
	Super::BeginPlay();
	commander = Cast<AAICommander>(UGameplayStatics::GetActorOfClass(GetWorld(), AAICommander::StaticClass()));
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
		if (commander != nullptr)
		{
			LevelActive = true;
			for (auto suben : en->LevelArray)
			{
				if (suben != this)
				{
					commander->ListReset(Cast<ASubEncounterSpace>(suben));
				}
			}
			commander->m_suben = this;
		}
		
	}
}


void ASubEncounterSpace::EnemyAICheck()
{
	this->GetOverlappingActors(M_AIArray,AAICharacter::StaticClass());
	for (auto AI : M_AIArray)
	{
		//INDEX_NONE
		if(AIArray.Find(AI) == INDEX_NONE)
		{
			AIController = Cast<AAI_Controller>(Cast<AAICharacter>(AI)->GetController());
			if (AIController)
			{
				if (AIController->GetBlackboardComponent())
				{
					if (AIController->GetBlackboardComponent()->GetValueAsBool("AI_Active"))
					{
						AIArray.Add(AI);
					}

				}
			}
		}
		
	}
}



