// Fill out your copyright notice in the Description page of Project Settings.


#include "CompassWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerStatComponent.h"
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
	Goal_Image->SetRenderTranslation(CalcGoalPos());
	//Goal_Slot->SetPosition(CalcGoalPos());
}

FVector2D UCompassWidget::CalcPointsPos()
{
	FVector2D pos;
	pos.Y = 0;
	pos.X = (GetOwningPlayer()->GetControlRotation().Yaw - 180) * -1.f * 10.f;
	return pos;
}

FVector2D UCompassWidget::CalcGoalPos()
{
	if (GetOwningPlayerPawn()->FindComponentByClass<UPlayerStatComponent>()->InteractActor)
	{
		FVector2D pos;

		FVector2D normal;
		FVector goalLoc = GetOwningPlayerPawn()->FindComponentByClass<UPlayerStatComponent>()->InteractActor->GetActorLocation();
		FVector pcLoc = GetOwningPlayerPawn()->GetActorLocation();
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(goalLoc, pcLoc);
		normal = FVector2D(rot.Vector());
		normal.Normalize();
		FVector2D(GetOwningPlayerPawn()->GetActorForwardVector());
		float fdot = FVector2D::DotProduct(FVector2D(GetOwningPlayerPawn()->GetActorForwardVector()), normal);
		float rdot = FVector2D::DotProduct(FVector2D(GetOwningPlayerPawn()->GetActorRightVector()), normal);
		float val = (rdot / fdot) * 700.f;
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, FString::SanitizeFloat(val));
		pos.Y = 0;
		pos.X = val;
		return pos;
	}
	return FVector2D::ZeroVector;
}
