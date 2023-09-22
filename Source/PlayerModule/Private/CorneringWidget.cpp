// Fill out your copyright notice in the Description page of Project Settings.


#include "CorneringWidget.h"
#include "CoverComponent.h"
#include "UMG.h"

void UCorneringWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCorneringWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


}

void UCorneringWidget::SetVisibleCorneringWidget(bool bvisible, bool bright)
{
	if (bvisible)
	{
		Cornering_Bar->SetRenderOpacity(1.0f);
		if (bright)
		{
			Cornering_Bar->SetRenderTransformAngle(.0f);
		}
		else
		{
			Cornering_Bar->SetRenderTransformAngle(180.0f);
		}
	}
	else
	{
		Cornering_Bar->SetRenderOpacity(0.0f);
	}
}

void UCorneringWidget::SetCorneringPercent(float percent)
{
	Cornering_Bar->SetPercent(percent);
}

