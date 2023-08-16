// Fill out your copyright notice in the Description page of Project Settings.


#include "AIZombie.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AAIZombie::AAIZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAIZombie::BeginPlay()
{
	Super::BeginPlay();
	
	player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

// Called every frame
void AAIZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), player->GetActorLocation());
}

// Called to bind functionality to input
void AAIZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

