// Fill out your copyright notice in the Description page of Project Settings.


#include "Crosshair_Widget.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetTree.h"
#include "PlayerWeaponComponent.h"
#include "UMG.h"

UCrosshair_Widget::UCrosshair_Widget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	
}

void UCrosshair_Widget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
}

void UCrosshair_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	m_alpha = 0;
	m_offset = 0;
	m_hitTime = 0;

	UTexture2D* texture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/yjs/UI/Textures/T_crosshair.T_crosshair'"));
	FSlateBrush brush;
	brush.SetResourceObject(texture);
	UOverlaySlot* hitslot;
	if (Hit_image)
	{
		hitslot = Cast<UOverlaySlot>(Hit_image->Slot);
		hitslot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		hitslot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

		Hit_image->SetBrush(brush);
		Hit_image->SetRenderScale(FVector2D(3.0f, 3.0f));
		Hit_image->SetRenderTransformAngle(45.0f);
		Hit_image->SetRenderOpacity(0.0f);
	}

	if (HeadHit_image)
	{
		hitslot = Cast<UOverlaySlot>(HeadHit_image->Slot);
		hitslot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		hitslot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

		brush.TintColor = FSlateColor(FColor::Red);
		HeadHit_image->SetBrush(brush);
		HeadHit_image->SetRenderScale(FVector2D(3.0f, 3.0f));
		HeadHit_image->SetRenderTransformAngle(45.0f);
		HeadHit_image->SetRenderOpacity(0.0f);
	}
	
}

void UCrosshair_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(m_alpha));
}

void UCrosshair_Widget::UpdateCrosshair(float DeltaTime)
{
	if (weapon)
	{
		if (CalcAlphaValue(DeltaTime))
		{
			weapon->m_firecount = 0;
		}
		CalcOffset(weapon->m_spreadPower);
		SetCrosshairTranslation();
		CheckHit(DeltaTime);
	}
}

void UCrosshair_Widget::SetCrosshairTranslation()
{
	FVector2D up = FVector2D(0, -m_offset);
	Up_Cross_image->SetRenderTranslation(up);

	FVector2D down = FVector2D(0, m_offset);
	Down_Cross_image->SetRenderTranslation(down);

	FVector2D left = FVector2D(-m_offset, 0);
	Left_Cross_image->SetRenderTranslation(left);

	FVector2D right = FVector2D(m_offset, 0);
	Right_Cross_image->SetRenderTranslation(right);
}

void UCrosshair_Widget::CalcOffset(float spreadpower)
{
	m_offset = FMath::Lerp(0.0f, spreadpower * 300.0f, m_alpha);
}

bool UCrosshair_Widget::CalcAlphaValue(float DeltaTime)
{
	if (weapon->bRecoil)
	{
		m_alpha += DeltaTime * 0.6f;
	}
	else
	{
		m_alpha -= DeltaTime * 0.4f;
	}

	if (m_alpha < 0)
	{
		m_alpha = 0;
		return true;
	}
	else if (m_alpha > 1)
	{
		m_alpha = 1;
	}
	return false;
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(m_alpha));
}

void UCrosshair_Widget::CheckHit(float delta)
{
	if (weapon->isHit)
	{
		if (weapon->headhit)
		{
			weapon->isHit = false;
			weapon->headhit = false;
			HeadHit_image->SetRenderOpacity(1.0f);
		}
		else
		{
			weapon->isHit = false;
			Hit_image->SetRenderOpacity(1.0f);
		}
	}

	if(Hit_image->RenderOpacity >= 1.0f || HeadHit_image->RenderOpacity >= 1.0f)
	{
		m_hitTime += delta;
		if (m_hitTime > 0.5f)
		{
			m_hitTime = 0;
			Hit_image->SetRenderOpacity(0.0f);
			HeadHit_image->SetRenderOpacity(0.0f);
		}
	}
}

