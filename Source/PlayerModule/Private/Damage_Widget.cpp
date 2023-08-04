// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage_Widget.h"
#include "Animation/WidgetAnimation.h"
#include "UMG.h"

void UDamage_Widget::NativeConstruct()
{
	Super::NativeConstruct();


}

void UDamage_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (addValue != FVector2D::ZeroVector)
	{
		SetWidgetLocation(InDeltaTime);
	}

}

void UDamage_Widget::SetDamageText(float value, FHitResult result)
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

	PlayAnimationForward(DamageAnim);

	GetWorld()->GetTimerManager().SetTimer(DamageTimer,
			FTimerDelegate::CreateLambda([&]()
				{
					RemoveFromViewport();
				}
		), 1.5f, false);

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
