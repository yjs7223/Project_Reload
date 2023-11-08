// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimeOutWidget.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UTimeOutWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void StartCount(int p_timeCount);

	void CountTime();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	class UOverlay* TimeOut_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	class UImage* TimeOut_Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	class UTextBlock* TimeOut_Text;

	int maxCount;
	int leftCount;
	FTimerHandle timeOutHandle;
};
