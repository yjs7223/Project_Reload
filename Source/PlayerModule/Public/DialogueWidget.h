// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetDialogueOpacity();

	UFUNCTION(BlueprintCallable)
	void SetDialogueText(FName rowName);

	void WriteDialogueText();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* Dialogue_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Dialogue_Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class URichTextBlock* Dialogue_RichText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UTextBlock* Dialogue_Name;

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
