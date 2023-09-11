// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharactorController.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API APlayerCharactorController : public APlayerController
{
	GENERATED_BODY()
public:
	APlayerCharactorController();

public:

	TObjectPtr<class UPathFollowingComponent> m_PathFollowingComp;
};
