// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "AICharacter.h"

UAIStatComponent::UAIStatComponent()
{
	
}

void UAIStatComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner<AAICharacter>();
}

void UAIStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAIStatComponent::Attacked1(float p_damage, FHitResult result)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("AIStat"));
	if (result.GetActor()->ActorHasTag("Enemy"))
	{
		/*if (result.GetActor()->FindComponentByClass<UCapsuleComponent>() == Cast<AAICharacter>(IndirectCollision)->CollisionMesh)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("AIstat"));
		}*/
	}
}
