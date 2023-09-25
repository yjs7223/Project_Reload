// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CorneringWidget.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UCorneringWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetVisibleCorneringWidget(bool bvisible, bool bright);

	void SetCorneringPercent(float percent);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UProgressBar* Cornering_Bar;
	
};
