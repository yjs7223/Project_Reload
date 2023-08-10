// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "CoverManager.generated.h"


/**
 * 
 */
UCLASS()
class PROJECT_RELOAD_API ACoverManager : public AInfo
{
	GENERATED_BODY()
	

public:
	ACoverManager();
	virtual void BeginPlay() override;

public:

	class AActor* CoverSystem;

	//Commander
	class AAICommander* commander;
	class TArray<AActor*> EncounterArray;

private:

};
