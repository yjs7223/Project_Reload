// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetVisibleInterface.h"
#include "Blueprint/UserWidget.h"
#include "LineNaviWidget.generated.h"



/**
 * 
 */
UCLASS()
class PLAYERMODULE_API ULineNaviWidget : public UUserWidget, public IWidgetVisibleInterface
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	void ArraySorting();

	void RemovePrevPoint(int index);

	virtual void SetWidgetVisible() override;

	void SetCoverLine(TArray<FNavPathPoint> p_CoverLocs);

public:
	TArray<AActor*> LinePoints;
	TArray<FVector2D> PointLocations;

	TArray<FVector2D> CoverLocs;

	bool bWidgetVisible;
	FTimerHandle VisibleTimer;
};
