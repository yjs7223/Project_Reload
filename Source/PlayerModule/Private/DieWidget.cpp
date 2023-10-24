// Fill out your copyright notice in the Description page of Project Settings.


#include "DieWidget.h"
#include "Animation/WidgetAnimation.h"
#include "PlayerStatComponent.h"
#include "UMG.h"

void UDieWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UPlayerStatComponent* statComp = GetOwningPlayerPawn()->FindComponentByClass<UPlayerStatComponent>())
	{
		statComp->diePlay.__Internal_AddDynamic(this, &UDieWidget::PlayDieAnim, FName("PlayDieAnim"));
	}
}

void UDieWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


}

void UDieWidget::PlayDieAnim()
{
	PlayAnimation(Die_Anim, .1f);
}
