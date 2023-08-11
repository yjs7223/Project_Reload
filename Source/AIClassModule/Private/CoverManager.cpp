// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverManager.h"
#include "CoverSystem.h"
#include "AICommander.h"
#include "SubEncounterSpace.h"
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

bool ACoverManager::ChangeEncounter()
{
	if (!commander->Now_suben)
		return false;

	NowSub = commander->Now_suben;

	this->SetActorLocation(NowSub->GetActorLocation());
	CoverSystem->SetActorLocation(NowSub->GetActorLocation());

	ChangeCoverSystem();
	//commander->RestoreArr();
	
	return true;
}

void ACoverManager::ChangeCoverSystem()
{

	//CoverSystem->GenerateCovers(true, false);
	CoverSystem->custom_GenerateCovers(true, false, true,2000);
		
}
