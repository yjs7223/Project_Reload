// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"
#include "PlayerCharacter.h"
#include "Player_HP_Widget.h"
#include "Player_Ammo_Widget.h"
#include "Crosshair_Widget.h"
#include "Attacked_Widget.h"
#include "Damage_Widget.h"
#include "LineNaviWidget.h"
#include "InteractiveWidget.h"
#include "PlayerWeaponComponent.h"
#include "PlayerInputComponent.h"
#include "Animation/WidgetAnimation.h"
#include "UMG.h"

void UPlayerHUDWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

}

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();


	InitWidgets();

	if (UPlayerWeaponComponent* weaponComp = GetOwningPlayerPawn()->FindComponentByClass<UPlayerWeaponComponent>())
	{
		weaponComp->OnSpawnDamageUIDelegate.BindUObject(this, &UPlayerHUDWidget::CreateDamageWidget);
	}

	if (UPlayerInputComponent* inputComp = GetOwningPlayerPawn()->FindComponentByClass<UPlayerInputComponent>())
	{
		inputComp->OnWidgetVisible.AddUObject(this, &UPlayerHUDWidget::SetWidgetVisible);
	}
}

void UPlayerHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPlayerHUDWidget::InitWidgets()
{
	if (!LineNaviWidget)
	{
		LineNaviWidget = CreateWidget<ULineNaviWidget>(GetOwningPlayer(), ULineNaviWidget::StaticClass());
		LineNaviWidget->AddToViewport();
	}
}

void UPlayerHUDWidget::CreateDamageWidget(float value, FHitResult result)
{
	if (Damage_WidgetClass)
	{
		UDamage_Widget* dwidget = CreateWidget<UDamage_Widget>(GetOwningPlayer(), Damage_WidgetClass);
		if (dwidget)
		{
			dwidget->SetDamageText(value, result);
			dwidget->AddToViewport();
		}
	}
}

void UPlayerHUDWidget::SetWidgetVisible(bool p_visible)
{
	if (FadeOutAnimation)
	{
		Player_HP_Widget->SetRenderOpacity(1.0f);
		Player_Ammo_Widget->SetRenderOpacity(1.0f);
		Crosshair_Widget->SetRenderOpacity(1.0f);

		if (IsAnimationPlaying(FadeOutAnimation))
		{
			StopAnimation(FadeOutAnimation);
		}
		if (p_visible)
		{
			PlayAnimationForward(FadeOutAnimation);
		}
	}
}
