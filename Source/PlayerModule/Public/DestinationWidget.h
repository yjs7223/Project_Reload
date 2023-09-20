// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DestinationWidget.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UDestinationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void ArraySorting();

	void SetTranslation();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* Des_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Des_image;


	TArray<AActor*> desActors;

	AActor* nowDes;
};
