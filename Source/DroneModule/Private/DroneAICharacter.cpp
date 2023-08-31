// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneAICharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Engine/CollisionProfile.h"	
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/Object.h"

// Sets default values
ADroneAICharacter::ADroneAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	//CharMovement = FindComponentByClass(UCharacterMovementComponent::StaticClass());


	CharMovement = GetCharacterMovement();
	CharMovement->DefaultLandMovementMode = EMovementMode::MOVE_Flying;
	CharMovement->MaxFlySpeed = 400.f;


	//Ä¸½¶ »çÀÌÁî º¯°æ ¾ÈµÊ?
	//GetCapsuleComponent()->SetCapsuleRadius(30);
	GetCapsuleComponent()->SetCapsuleHalfHeight(35.0f);
	
	//¸Þ½¬ Ãß°¡
	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMesh"));
	m_mesh->SetupAttachment(GetMesh(), TEXT("DroneMesh"));

}

// Called when the game starts or when spawned
void ADroneAICharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ADroneAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADroneAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

