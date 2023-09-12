// Fill out your copyright notice in the Description page of Project Settings.


#include "CompassWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerStatComponent.h"
#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
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
			MyStatComp->OnCreateAttackedUIDelegate.BindUObject(this, &UCompassWidget::AddEnemy);
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
	if (GetOwningPlayerPawn()->FindComponentByClass<UPlayerStatComponent>()->InteractActor)
	{
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

void UCompassWidget::CalcEnemysPos()
{
	if (Enemys.Num() == 0)
	{
		return;
	}

	FVector2D pos = FVector2D::ZeroVector;
	UCameraComponent* camera = GetOwningPlayerPawn()->FindComponentByClass<UCameraComponent>();
	TArray<ABaseCharacter*> deleteLists;
	if (camera)
	{
		for (int i = 0; i < Enemys.Num(); i++)
		{
			if (!Enemys[i]->FindComponentByClass<UStatComponent>()->bDie)
			{
				FVector2D normal;
				FVector goalLoc = Enemys[i]->GetActorLocation();
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
				pos.Y = 53;
				pos.X = val;
				Cast<UCanvasPanelSlot>(EnemyPoints[i]->Slot)->SetPosition(pos);
			}
			else
			{
				deleteLists.Add(Enemys[i]);
			}
		}
	}

	for (int i = 0; i < deleteLists.Num(); i++)
	{
		if (Enemys.Find(deleteLists[i]) != INDEX_NONE)
		{
			Enemys.Remove(deleteLists[i]);
			EnemyPoints.RemoveAt(0);
		}
	}

}

void UCompassWidget::AddEnemy(ABaseCharacter* enemy)
{
	if (Enemys.Find(enemy) == INDEX_NONE)
	{
		Enemys.Add(enemy);
	}
	else
	{
		return;
	}

	if (EnemyPointClass)
	{
		UUserWidget* epoint = CreateWidget<UUserWidget>(GetOwningPlayer(), EnemyPointClass);
		Compass_Canvas->AddChildToCanvas(epoint);
		Cast<UCanvasPanelSlot>(epoint->Slot)->SetSize(FVector2D(15.f, 10.f));
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
