// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveComponent.h"

// Sets default values for this component's properties
UInteractiveComponent::UInteractiveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractiveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UInteractiveComponent::BeginDestroy()
{
	OnInteractDelegate.Unbind();

	Super::BeginDestroy();
}


// Called every frame
void UInteractiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractiveComponent::ActiveInteractable()
{
	if (bActive)
	{
		bActive = false;
	}
	else
	{
		bActive = true;
	}
}

void UInteractiveComponent::StartInteract()
{
	bInteractive = true;
	bActive = false;

	OnInteractDelegate.ExecuteIfBound();
	
}

