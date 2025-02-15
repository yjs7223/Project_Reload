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
#include "DestinationWidget.h"
#include "PauseWidget.h"
#include "CorneringWidget.h"
#include "TimeOutWidget.h"
#include "Player_Cover_Widget.h"
#include "PlayerWeaponComponent.h"
#include "PlayerStatComponent.h"
#include "PlayerInputComponent.h"
#include "CoverComponent.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "UMG.h"

void UPlayerHUDWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
}

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//damageTextSize = 16;
	InitWidgets();

	if (UPlayerWeaponComponent* weaponComp = GetOwningPlayerPawn()->FindComponentByClass<UPlayerWeaponComponent>())
	{
		weaponComp->OnSpawnDamageUIDelegate.BindUObject(this, &UPlayerHUDWidget::CreateDamageWidget);
		weaponComp->OnCombatWidgetVisible.AddUObject(this, &UPlayerHUDWidget::SetCombatWidgetVisible);
	}

	if (UPlayerInputComponent* inputComp = GetOwningPlayerPawn()->FindComponentByClass<UPlayerInputComponent>())
	{
		inputComp->OnAllWidgetVisible.AddUObject(this, &UPlayerHUDWidget::SetAllWidgetVisible);
		inputComp->OnCreatePauseWidget.BindUObject(this, &UPlayerHUDWidget::CreatePauseWidget);
	}

	if (UPlayerStatComponent* statComp = GetOwningPlayerPawn()->FindComponentByClass<UPlayerStatComponent>())
	{
		statComp->diePlay.__Internal_AddDynamic(this, &UPlayerHUDWidget::DeactiveWidget, FName("DeactiveWidget"));
	}

	if (UCoverComponent* coverComp = GetOwningPlayerPawn()->FindComponentByClass<UCoverComponent>())
	{
		coverComp->OnFaceRightCoverWidget.BindUObject(this, &UPlayerHUDWidget::SetFaceRightWidget);
	}
}

void UPlayerHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


	SetCorneringTranslation();
}

void UPlayerHUDWidget::InitWidgets()
{
	if (!LineNaviWidget)
	{
		LineNaviWidget = CreateWidget<ULineNaviWidget>(GetOwningPlayer(), ULineNaviWidget::StaticClass());
		LineNaviWidget->AddToViewport();
	}
}

void UPlayerHUDWidget::CreateDamageWidget(float value, FHitResult result, bool p_bHead)
{
	if (Damage_WidgetClass)
	{
		UDamage_Widget* dwidget = CreateWidget<UDamage_Widget>(GetOwningPlayer(), Damage_WidgetClass);
		if (dwidget)
		{
			if (damageTextSize <= 0)
			{
				damageTextSize = 16;
			}
			dwidget->SetDamageText(value, result, p_bHead, damageTextSize);
			dwidget->AddToViewport();
		}
	}
}

void UPlayerHUDWidget::SetCombatWidgetVisible(bool p_visible)
{
	if (CombatFadeOutAnimation)
	{
		Player_HP_Widget->SetRenderOpacity(1.0f);
		Player_Ammo_Widget->SetRenderOpacity(1.0f);
		Player_HP_Widget_L->SetRenderOpacity(1.0f);
		Player_Ammo_Widget_L->SetRenderOpacity(1.0f);
		Crosshair_Widget->SetRenderOpacity(1.0f);
		//DestinationWidget->SetRenderOpacity(1.0f);

		if (IsAnimationPlaying(CombatFadeOutAnimation))
		{
			StopAnimation(CombatFadeOutAnimation);
		}
		if (p_visible)
		{
			PlayAnimationForward(CombatFadeOutAnimation);
		}
	}
}

void UPlayerHUDWidget::SetAllWidgetVisible(bool p_visible)
{
	if (CombatFadeOutAnimation)
	{
		Player_HP_Widget->SetRenderOpacity(1.0f);
		Player_Ammo_Widget->SetRenderOpacity(1.0f);
		Player_HP_Widget_L->SetRenderOpacity(1.0f);
		Player_Ammo_Widget_L->SetRenderOpacity(1.0f);
		Crosshair_Widget->SetRenderOpacity(1.0f);
		DestinationWidget->SetRenderOpacity(1.0f);

		if (IsAnimationPlaying(CombatFadeOutAnimation))
		{
			StopAnimation(CombatFadeOutAnimation);
		}
		if (p_visible)
		{
			PlayAnimationForward(CombatFadeOutAnimation);
		}

		if (IsAnimationPlaying(DesFadeOutAnimation))
		{
			StopAnimation(DesFadeOutAnimation);
		}
		if (p_visible)
		{
			PlayAnimationForward(DesFadeOutAnimation);
		}
	}
}

void UPlayerHUDWidget::CreatePauseWidget()
{
	if (Pause_WidgetClass)
	{
		UPauseWidget* pwidget = CreateWidget<UPauseWidget>(GetOwningPlayer(), Pause_WidgetClass);
		if (pwidget)
		{
			pwidget->AddToViewport();
		}
	}
}

void UPlayerHUDWidget::SetCorneringTranslation()
{
	if (CorneringWidget)
	{
		FVector2D loc;
		if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), GetOwningPlayerPawn()->GetActorLocation(), loc, true))
		{
			CorneringWidget->SetRenderTranslation(loc);
		}
	}
}

void UPlayerHUDWidget::SetFaceRightWidget(bool p_bFaceright)
{
	if (p_bFaceright)
	{
		Right_Overlay->SetRenderOpacity(1.0f);
		Left_Overlay->SetRenderOpacity(0.0f);
	}
	else
	{
		Right_Overlay->SetRenderOpacity(0.0f);
		Left_Overlay->SetRenderOpacity(1.0f);
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="p_bFadeIn">: fadeIn => true, FadeOut => false </param>
void UPlayerHUDWidget::PlayFadeInOutAnim(bool p_bFadeIn)
{
	if (FullFadeInAnimation && FullFadeOutAnimation)
	{
		if (p_bFadeIn)
		{
			PlayAnimation(FullFadeInAnimation);
		}
		else
		{
			PlayAnimation(FullFadeOutAnimation);
		}
	}
}

void UPlayerHUDWidget::DeactiveWidget()
{
	Player_HP_Widget->SetRenderOpacity(0.0f);
	Player_Ammo_Widget->SetRenderOpacity(0.0f);
	Player_HP_Widget_L->SetRenderOpacity(0.0f);
	Player_Ammo_Widget_L->SetRenderOpacity(0.0f);
}
