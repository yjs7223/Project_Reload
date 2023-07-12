// Fill out your copyright notice in the Description page of Project Settings.


#include "SCrossHairWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/Images/SImage.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SCrossHairWidget::Construct(const FArguments& InArgs)
{
	const FMargin contentPadding = FMargin(500.0f, 300.0f);

	ChildSlot
	[
		SNew(SOverlay) + SOverlay::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
		[
			SNew(SImage)
			.ColorAndOpacity(FColor::White)
			.DesiredSizeOverride(FVector2D(8.0f, 8.0f))
			.RenderTransform(FSlateRenderTransform(FScale2D(0.5f)))
			.RenderTransformPivot(FVector2D(0.5f,0.5f))
			
		]

	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
