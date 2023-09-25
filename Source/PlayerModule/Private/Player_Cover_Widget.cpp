// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Cover_Widget.h"
#include "CoverComponent.h"
#include "UMG.h"

void UPlayer_Cover_Widget::NativeConstruct()
{
	if (UCoverComponent* covercomp = GetOwningPlayerPawn()->FindComponentByClass<UCoverComponent>())
	{
		covercomp->OnVisibleCoverWidget.BindUObject(this, &UPlayer_Cover_Widget::SetVisibleCoverWidget);
	}
}

void UPlayer_Cover_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
}

void UPlayer_Cover_Widget::SetVisibleCoverWidget(FVector point)
{
	if (point == FVector::ZeroVector)
	{
		Cover_Overlay->SetRenderOpacity(.0f);
	}
	else
	{
		Cover_Overlay->SetRenderOpacity(1.0f);
	}
}
