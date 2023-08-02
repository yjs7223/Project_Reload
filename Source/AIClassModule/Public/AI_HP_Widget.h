// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AI_HP_Widget.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API UAI_HP_Widget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetDelegate(class AAICharacter* character);

	void SwitchVisibleWidget();

	void SetPercent();
	void SetBackPercent(float deltatime);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAIStatComponent* stat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* HP_Overlay_s;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* HP_Image_s1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* HP_Image_s2;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* HP_Overlay_l;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* HP_Image_l;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* HP_Image_Back;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UProgressBar* HP_Bar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UProgressBar* HP_BackBar;


public:
	FTimerHandle switchTimer;
	FTimerHandle dieTimer;
};
