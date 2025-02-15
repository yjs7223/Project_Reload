// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage_Widget.h"
#include "Animation/WidgetAnimation.h"
#include "UMG.h"

void UDamage_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	Damage_Overlay->SetRenderOpacity(1.0f);
}

void UDamage_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (addValue != FVector2D::ZeroVector)
	{
		SetWidgetLocation(InDeltaTime);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("create"));
}

void UDamage_Widget::SetDamageText(float value, FHitResult result)
{
	if (this)
	{
		GetWorld()->GetTimerManager().ClearTimer(DamageTimer);

		hitLocation = result.Location;
		addValue.X = FMath::RandRange(30.f, 50.f);
		addValue.Y = FMath::RandRange(-30.f, -50.f);

		UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), hitLocation, damageLocation);
		FIntVector2 screensize;
		GetOwningPlayer()->GetViewportSize(screensize.X, screensize.Y);
		damageLocation.X -= screensize.X / 2;
		damageLocation.Y -= screensize.Y / 2;
		damageLocation.X += addValue.X;
		damageLocation.Y += addValue.Y;

		Damage_Overlay->SetRenderTranslation(damageLocation);

		Damage_Text->SetText(FText::FromString(FString::FromInt(value)));

		if (DamageAnim)
		{
			PlayAnimationForward(DamageAnim);
		}
		GetWorld()->GetTimerManager().SetTimer(DamageTimer,
			FTimerDelegate::CreateLambda([&]()
				{
					RemoveFromViewport();
				}
		), 1.5f, false);
	}
}

void UDamage_Widget::SetDamageText(float value, FHitResult result, bool p_bHead, int p_size)
{
	SetDamageText(value, result);

	if (p_bHead)
	{
		FSlateColor c = FSlateColor(FColor::Red);
		Damage_Text->SetColorAndOpacity(c);
		Damage_Text->Font.Size = p_size;
		Cast<UCanvasPanelSlot>(Damage_Overlay->Slot)->SetSize(FVector2D(50.f, p_size * 1.5f));
	}
}

void UDamage_Widget::SetWidgetLocation(float InDeltaTime)
{
	UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), hitLocation, damageLocation);
	FIntVector2 screensize;
	GetOwningPlayer()->GetViewportSize(screensize.X, screensize.Y);
	damageLocation.X -= screensize.X / 2;
	damageLocation.Y -= screensize.Y / 2;

	addValue.Y -= InDeltaTime * 50;
	damageLocation.X += addValue.X;
	damageLocation.Y += addValue.Y;

	Damage_Overlay->SetRenderTranslation(damageLocation);
}
