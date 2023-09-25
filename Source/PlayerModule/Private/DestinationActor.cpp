// Fill out your copyright notice in the Description page of Project Settings.


#include "DestinationActor.h"
#include "Components/BoxComponent.h"

// Sets default values
ADestinationActor::ADestinationActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

}

// Called when the game starts or when spawned
void ADestinationActor::BeginPlay()
{
	Super::BeginPlay();
	

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ADestinationActor::OnBoxBeginOverlap);
}

// Called every frame
void ADestinationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ADestinationActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (OtherActor->Tags.Num() > 0)
		{
			if (OtherActor->ActorHasTag("Player"))
			{
				OnUpdateDesUIDelegate.ExecuteIfBound(this);
			}
		}
	}
}

