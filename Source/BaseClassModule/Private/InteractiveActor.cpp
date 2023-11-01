// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActor.h"
#include "InteractiveComponent.h"



// Sets default values
AInteractiveActor::AInteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractiveComp = CreateDefaultSubobject<UInteractiveComponent>(TEXT("InteractComp"));
	if (InteractiveComp)
	{
		InteractiveComp->OnInteractDelegate.BindUObject(this, &AInteractiveActor::Interact);
	}
	staticmesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
}

// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


