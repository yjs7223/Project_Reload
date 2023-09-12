// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharactorController.h"
#include "Navigation/PathFollowingComponent.h"

APlayerCharactorController::APlayerCharactorController()
{
	m_PathFollowingComp = CreateDefaultSubobject<UPathFollowingComponent>(TEXT("PathFollowing"));
}
