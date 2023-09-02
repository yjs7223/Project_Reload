// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverManager.h"
#include "CoverSystem.h"
#include "AICommander.h"
#include "EncounterSpace.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/BoxSphereBounds.h"
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
	if (!commander->Now_en)
		return false;

	NowEn = commander->Now_en;

	this->SetActorLocation(NowEn->GetActorLocation());
	CoverSystem->SetActorLocation(NowEn->GetActorLocation());

	ChangeCoverSystem();
	commander->CoverPointEn(NowEn);
	//commander->RestoreArr();
	
	return true;
}

void ACoverManager::ChangeCoverSystem()
{

	//CoverSystem->GenerateCovers(true, false);
	CoverSystem->custom_GenerateCovers(true, false, true,2500);
	TArray<FCoverHandle> testarray;
	CoverSystem->GetCoversWithinBounds(FBoxSphereBounds(FVector3d(CoverSystem->GetActorLocation()), FVector3d(10000.0f, 10000.0f, 10000.0f), 10000.0f), testarray);
	if (!testarray.IsEmpty())
	{
		commander->CoverArray.Reset();
		for (auto item : testarray)
		{
			FCoverData coverdata;
			CoverSystem->GetCoverData(item, coverdata);
			commander->CoverArray.Add(coverdata.Location);
		}
	}
}
