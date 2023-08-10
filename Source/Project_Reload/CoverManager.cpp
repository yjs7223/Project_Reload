// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverManager.h"
#include "CoverSystem.h"
#include "AICommander.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

ACoverManager::ACoverManager()
{

}

void ACoverManager::BeginPlay()
{
	CoverSystem = Cast<ACoverSystem>
		(UGameplayStatics::GetActorOfClass(GetWorld(), ACoverSystem::StaticClass()));
	commander = Cast<AAICommander>
		(UGameplayStatics::GetActorOfClass(GetWorld(), AAICommander::StaticClass()));







}