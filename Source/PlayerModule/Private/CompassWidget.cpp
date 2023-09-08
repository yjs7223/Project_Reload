// Fill out your copyright notice in the Description page of Project Settings.


#include "CompassWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "UMG.h"

void UCompassWidget::NativeConstruct()
{
	Super::NativeConstruct();


	Points_Slot = Cast<UCanvasPanelSlot>(Points_Image->Slot);
}

void UCompassWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


	Points_Slot->SetPosition(CalcPointsPos());
}

FVector2D UCompassWidget::CalcPointsPos()
{
	FVector2D pos;
	pos.Y = 0;
	pos.X = (GetOwningPlayer()->GetControlRotation().Yaw - 180) * -1.f * 10.f;
	GEngine->AddOnScreenDebugMessage(-1, 0.02f, FColor::Red, FString::SanitizeFloat(pos.X));
	return pos;
}
