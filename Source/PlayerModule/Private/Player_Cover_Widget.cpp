// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Cover_Widget.h"
#include "UMG.h"

void UPlayer_Cover_Widget::NativeConstruct()
{

}

void UPlayer_Cover_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
}

void UPlayer_Cover_Widget::SetOpacity(float opacity)
{
	if (Cover_Overlay)
	{
		Cover_Overlay->SetRenderOpacity(opacity);
	}
}
