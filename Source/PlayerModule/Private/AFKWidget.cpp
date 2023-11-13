// Fill out your copyright notice in the Description page of Project Settings.


#include "AFKWidget.h"
#include "MediaPlayer.h"
#include "MediaPlaylist.h"
#include "UMG.h"

void UAFKWidget::NativeConstruct()
{
	Super::NativeConstruct();

	afkTime = 0;

}

void UAFKWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetOwningPlayer()->WasInputKeyJustPressed(EKeys::AnyKey))
	{
		afkTime = 0;
		bAfk = false;
		AFK_Overlay->SetRenderOpacity(0.f);
		StopMovie();
		//movie stop
	}

	afkTime += InDeltaTime;
	
	if (afkTime >= 5.f)
	{
		bAfk = true;
		AFK_Overlay->SetRenderOpacity(1.f);
		PlayMovie();
		//movie play
	}
}

void UAFKWidget::PlayMovie()
{
	int num = afkMovies->Num();
	UMediaSource* movie = afkMovies->GetRandom(num);
	afkMoviePlayer->OpenSource(movie);
	afkMoviePlayer->Play();

}

void UAFKWidget::StopMovie()
{
	afkMoviePlayer->Close();
}
