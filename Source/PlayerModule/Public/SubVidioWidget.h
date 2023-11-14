// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SubVidioWidget.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API USubVidioWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void PlayMovie(int p_num);

	void StopMovie();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	class UOverlay* AFK_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	class UImage* AFK_Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMediaPlayer* afkMoviePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMediaTexture* afkMovieTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMediaPlaylist* afkMovies;

};
