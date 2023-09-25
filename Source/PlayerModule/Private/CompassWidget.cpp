// Fill out your copyright notice in the Description page of Project Settings.


#include "CompassWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerStatComponent.h"
#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnemyPointWidget.h"
#include "UMG.h"

void UCompassWidget::NativeConstruct()
{
	Super::NativeConstruct();


	Points_Slot = Cast<UCanvasPanelSlot>(Points_Image->Slot);
	Goal_Slot = Cast<UCanvasPanelSlot>(Goal_Image->Slot);

	if (APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn()))
	{
		if (UPlayerStatComponent* MyStatComp = Cast<UPlayerStatComponent>(MyCharacter->stat))
		{
			PlayerCamera = MyCharacter->FindComponentByClass<UCameraComponent>();
			MyStatComp->OnCreateAttackedUIDelegate.AddUObject(this, &UCompassWidget::AddEnemyPoint);
		}
	}
}

void UCompassWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


	CalcPointsPos();
	CalcGoalPos();
	CalcEnemysPos();
}

void UCompassWidget::CalcPointsPos()
{
	FVector2D pos;
	pos.Y = 0;
	pos.X = (GetOwningPlayer()->GetControlRotation().Yaw - 180) * -1.f * 10.f;
	Points_Slot->SetPosition(pos);
}

void UCompassWidget::CalcGoalPos()
{
	if (goalActor)
	{
		Goal_Image->SetRenderOpacity(1.0f);
		FVector2D pos = FVector2D::ZeroVector;
		if (PlayerCamera)
		{
			FVector2D normal;

			//goalobj로 대체예정
			FVector goalLoc = goalActor->GetActorLocation();
			FVector cameraLoc = PlayerCamera->GetComponentLocation();
			FRotator rot = UKismetMathLibrary::FindLookAtRotation(goalLoc, cameraLoc);
			if (CheckIfBehind(PlayerCamera->GetForwardVector(), rot.Vector()))
			{
				Goal_Image->SetRenderOpacity(0.0f);
				return;
			}
			normal = FVector2D(rot.Vector());
			normal.Normalize();
			float fdot = FVector2D::DotProduct(FVector2D(PlayerCamera->GetForwardVector()), normal);
			float rdot = FVector2D::DotProduct(FVector2D(PlayerCamera->GetRightVector()), normal);
			float val = (rdot / fdot) * 540.f;
			pos.Y = -75; 
			pos.X = val;
			Goal_Slot->SetPosition(pos);
		}
	}
	else
	{
		Goal_Image->SetRenderOpacity(0.0f);
	}
}

void UCompassWidget::CalcEnemysPos()
{
	
	if (PlayerCamera)
	{
		FVector2D pos = FVector2D::ZeroVector;

		for (int i = 0; i < EnemyPoints.Num(); i++)
		{
			if (EnemyPoints[i]->TargetEnemy)
			{
				FVector2D normal;
				FVector goalLoc = EnemyPoints[i]->TargetEnemy->GetActorLocation();
				FVector cameraLoc = PlayerCamera->GetComponentLocation();
				FRotator rot = UKismetMathLibrary::FindLookAtRotation(goalLoc, cameraLoc);
				if (CheckIfBehind(PlayerCamera->GetForwardVector(), rot.Vector()))
				{
					return;
				}
				normal = FVector2D(rot.Vector());
				normal.Normalize();
				float fdot = FVector2D::DotProduct(FVector2D(PlayerCamera->GetForwardVector()), normal);
				float rdot = FVector2D::DotProduct(FVector2D(PlayerCamera->GetRightVector()), normal);
				float val = (rdot / fdot) * 540.f;
				pos.Y = -14;
				pos.X = val;
				Cast<UCanvasPanelSlot>(EnemyPoints[i]->Slot)->SetPosition(pos);
			}
		}
	}
}

void UCompassWidget::AddEnemyPoint(ABaseCharacter* enemy)
{
	if (EnemyPointClass)
	{
		if (EnemyPoints.Num() > 0)
		{
			for (int i = 0; i < EnemyPoints.Num(); i++)
			{
				if (EnemyPoints[i]->TargetEnemy == enemy)
				{
					EnemyPoints[i]->StartAttacked(enemy);
					return;
				}
			}

			for (int i = 0; i < EnemyPoints.Num(); i++)
			{
				if (EnemyPoints[i]->TargetEnemy == nullptr)
				{
					EnemyPoints[i]->StartAttacked(enemy);
					return;
				}
			}
		}

		UEnemyPointWidget* epoint = CreateWidget<UEnemyPointWidget>(GetOwningPlayer(), EnemyPointClass);
		if (epoint->GetParent() != Compass_Canvas)
		{
			Compass_Canvas->AddChildToCanvas(epoint);
			//epoint->AddToViewport();
		}
		Cast<UCanvasPanelSlot>(epoint->Slot)->SetSize(FVector2D(30.f, 20.f));
		Cast<UCanvasPanelSlot>(epoint->Slot)->SetAnchors(FAnchors(0.5f, 0.5f));
		Cast<UCanvasPanelSlot>(epoint->Slot)->SetAlignment(FVector2D(0.5f, 0.5f));
		epoint->StartAttacked(enemy);
		EnemyPoints.Add(epoint);
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
	}
}
