// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacterMoveComponent.h"

// Sets default values for this component's properties
UAICharacterMoveComponent::UAICharacterMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAICharacterMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAICharacterMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (AI_Move)
	{
		if (Move_Normal)
		{
			
		}
		else if (Move_Attack)
		{

		}
		else if (Move_Hit)
		{

		}
	}
	// ...
}

