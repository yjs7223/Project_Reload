// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "SCrossHairWidget.h"

APlayerHUD::APlayerHUD()
{
	//
}

void APlayerHUD::BeginPlay()
{
	GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(crosshair, SCrossHairWidget));
}
