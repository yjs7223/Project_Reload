// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage_Widget.h"
#include "UMG.h"

void UDamage_Widget::NativeConstruct()
{
}

void UDamage_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
}

void UDamage_Widget::SetDamageText(float value)
{
	Damage_Text->SetText(FText::FromString(FString::FromInt(value)));
}
