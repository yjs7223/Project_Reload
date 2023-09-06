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
	/*if (Damage_WidgetClass)
	{
		UDamage_Widget* dwidget = CreateWidget<UDamage_Widget>(Cast<APlayerController>(GetController()), Damage_WidgetClass);
		if (dwidget)
		{
			dwidget->SetDamageText(value, result);
			dwidget->AddToViewport();
		}
	}*/
}
