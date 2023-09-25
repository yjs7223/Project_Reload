// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UMG.h"


void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Pause_Button->OnClicked.AddDynamic(this, &UPauseWidget::StopPause);
	Exit_Button->OnClicked.AddDynamic(this, &UPauseWidget::ExitGame);
	GetOwningPlayer()->SetShowMouseCursor(true);

}

void UPauseWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


}

void UPauseWidget::StopPause()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	GetOwningPlayer()->SetShowMouseCursor(false);
	RemoveFromViewport();
}

void UPauseWidget::ExitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
