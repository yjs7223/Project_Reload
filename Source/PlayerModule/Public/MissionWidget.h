// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionWidget.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UMissionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetMissionText(FName rowName);

	void WriteMissionText();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* Mission_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Mission_Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class URichTextBlock* Mission_RichText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UTextBlock* Mission_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float writingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bHighlight;

public:
	FName nowRowName;
	FString fullLine;
	int32 lineLen;
	int32 nowLen;
	FTimerHandle WritingTimer;
};
