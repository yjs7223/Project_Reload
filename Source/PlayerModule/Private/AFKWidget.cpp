// Fill out your copyright notice in the Description page of Project Settings.


#include "AFKWidget.h"
#include "MediaPlayer.h"
#include "MediaPlaylist.h"
#include "MediaTexture.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UMG.h"

void UAFKWidget::NativeConstruct()
{
	Super::NativeConstruct();


	AFK_Overlay->SetRenderOpacity(0.f);
	afkTime = 0;
	afkMoviePlayer->PlayOnOpen = true;

	//movieMat = LoadObject<UMaterialInterface>(NULL, TEXT("Material'/Game/Movies/M_VidioMat.M_VidioMat'"));
	if (movieMat)
	{
		movieMatinst = UMaterialInstanceDynamic::Create(movieMat, this);
		if (movieMatinst)
		{
			//FSlateBrush imageBrush;
			//imageBrush.ImageSize = FVector2D(30.0f, 30.0f);
			movieMatinst->SetTextureParameterValue(FName(TEXT("MovieTexture")), afkMovieTexture);
			//imageBrush.SetResourceObject(DynMaterial);
			AFK_Image->SetBrushFromMaterial(movieMatinst);

		}
	}
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
	
	if (afkTime >= 60.f && !bAfk)
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

	afkMoviePlayer->PlayAndSeek();

}

void UAFKWidget::StopMovie()
{
	afkMoviePlayer->Close();
}
