// Fill out your copyright notice in the Description page of Project Settings.


#include "CompassWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerStatComponent.h"
#include "Camera/CameraComponent.h"
#include "UMG.h"

void UCompassWidget::NativeConstruct()
{
	Super::NativeConstruct();


	Points_Slot = Cast<UCanvasPanelSlot>(Points_Image->Slot);
	Goal_Slot = Cast<UCanvasPanelSlot>(Goal_Image->Slot);
}

void UCompassWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


	CalcPointsPos();
	CalcGoalPos();
}

void UCompassWidget::CalcPointsPos()
{
	FVector2D pos;
	pos.Y = 0;
	pos.X = (GetOwningPlayer()->GetControlRotation().Yaw - 180) * -1.f * 10.f;
<<<<<<< Updated upstream
	return pos;
=======
	
	Points_Slot->SetPosition(pos);
>>>>>>> Stashed changes
}

void UCompassWidget::CalcGoalPos()
{
	if (GetOwningPlayerPawn()->FindComponentByClass<UPlayerStatComponent>()->InteractActor)
	{
<<<<<<< Updated upstream
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
=======
		FVector2D pos = FVector2D::ZeroVector;
		UCameraComponent* camera = GetOwningPlayerPawn()->FindComponentByClass<UCameraComponent>();
		if (camera)
		{
			FVector2D normal;
			FVector goalLoc = GetOwningPlayerPawn()->FindComponentByClass<UPlayerStatComponent>()->InteractActor->GetActorLocation();
			FVector cameraLoc = camera->GetComponentLocation();
			FRotator rot = UKismetMathLibrary::FindLookAtRotation(goalLoc, cameraLoc);
			if (CheckIfBehind(camera->GetForwardVector(), rot.Vector()))
			{
				return;
			}
			normal = FVector2D(rot.Vector());
			normal.Normalize();
			float fdot = FVector2D::DotProduct(FVector2D(camera->GetForwardVector()), normal);
			float rdot = FVector2D::DotProduct(FVector2D(camera->GetRightVector()), normal);
			float val = (rdot / fdot) * 540.f;
			pos.Y = -75; 
			pos.X = val;
			Goal_Slot->SetPosition(pos);
		}
	}
}

bool UCompassWidget::CheckIfBehind(FVector cameraForward, FVector lookatNormal)
{
	float val = FVector::DotProduct(lookatNormal, cameraForward);
	float c = UKismetMathLibrary::DegAcos(val);
	if (c < 120.0f)
	{
		return true;
	}
	else
	{
		return false;
>>>>>>> Stashed changes
	}
}
