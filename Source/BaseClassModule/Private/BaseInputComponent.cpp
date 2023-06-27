// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInputComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UBaseInputComponent::UBaseInputComponent()
{

}


// Called when the game starts
void UBaseInputComponent::BeginPlay()
{
	Super::BeginPlay();

	owner = Cast<ACharacter>(GetOwner());
}

FInputData* UBaseInputComponent::getInput()
{
	return &m_inputData;
}
