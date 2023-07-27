// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player_Cover_Widget.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UPlayer_Cover_Widget : public UUserWidget
{
	GENERATED_BODY()
		
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetOpacity(float opacity);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* Cover_Overlay;

public:

};
