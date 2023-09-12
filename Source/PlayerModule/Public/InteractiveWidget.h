// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractiveWidget.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UInteractiveWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void ActiveInteractWidget(bool p_bActive, AActor* InterActor);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* Interactive_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Interactive_Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* InteractiveObj;

};
