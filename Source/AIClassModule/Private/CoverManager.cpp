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
	PrimaryActorTick.bCanEverTick = true;
	ArrayActive = false;
}

void ACoverManager::BeginPlay()
{
	CoverSystem = Cast<ACoverSystem>
		(UGameplayStatics::GetActorOfClass(GetWorld(), ACoverSystem::StaticClass()));
	commander = Cast<AAICommander>
		(UGameplayStatics::GetActorOfClass(GetWorld(), AAICommander::StaticClass()));
}

void ACoverManager::Tick(float DeltaTime)
{
	if (!ArrayActive)
	{
		CoverPointArray();
	}
}

bool ACoverManager::ChangeEncounter()
{
	if (!commander->Now_en)
		return false;

	NowEn = commander->Now_en;

	//this->SetActorLocation(NowEn->GetActorLocation());
	//CoverSystem->SetActorLocation(NowEn->GetActorLocation());
	
	
	commander->CoverPointEn(NowEn);

	//commander->RestoreArr();
	
	return true;
}

void ACoverManager::CoverPointArray()
{
	TArray<FCoverHandle> CoverManagerArray;
	CoverSystem->GetCoversWithinBounds(FBoxSphereBounds(FVector3d(CoverSystem->GetActorLocation()), FVector3d(20000.0f, 20000.0f, 20000.0f), 20000.0f), CoverManagerArray);
	if (!CoverManagerArray.IsEmpty())
	{
		commander->CoverArray.Reset();
		for (auto item : CoverManagerArray)
		{
			FCoverData coverdata;
			CoverSystem->GetCoverData(item, coverdata);
			commander->CoverArray.Add(coverdata.Location);
		}
		ArrayActive = true;
	}
}
