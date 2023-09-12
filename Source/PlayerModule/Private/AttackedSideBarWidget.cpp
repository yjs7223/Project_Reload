// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackedSideBarWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/WidgetAnimation.h"
#include "PlayerCharacter.h"
#include "PlayerStatComponent.h"
#include "UMG.h"


void UAttackedSideBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UAttackedSideBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (TargetEnemy)
	{
		SetAttackedAngle();
	}
}

void UAttackedSideBarWidget::SetAttackedAngle()
{
	if (this)
	{
		if (TargetEnemy)
		{
			FVector enemyloc = TargetEnemy->GetActorLocation();
			if (enemyloc != FVector::ZeroVector)
			{
				FRotator rot = UKismetMathLibrary::FindLookAtRotation(enemyloc, GetOwningPlayerPawn()->GetActorLocation());
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, rot.ToString());
				if (Attacked_Overlay)
				{
					float control_rot = GetOwningPlayer()->GetControlRotation().Yaw - 180;
					Attacked_Overlay->SetRenderTransformAngle(rot.Yaw - control_rot);
				}

			}

			if (UStatComponent* Estat = TargetEnemy->FindComponentByClass<UStatComponent>())
			{
				if (Estat->bDie)
				{
					TargetEnemy = nullptr;
					Attacked_Overlay->SetRenderOpacity(0.0f);
				}
			}
		}
	}
}

void UAttackedSideBarWidget::StartAttacked(ABaseCharacter* Target)
{
	if (this)
	{
		TargetEnemy = Target;

		FVector2D loc;
		FIntVector2 screensize;
		GetOwningPlayer()->GetViewportSize(screensize.X, screensize.Y);
		loc.X = 0;
		loc.Y = (screensize.Y / 2);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, loc.ToString());
		Attacked_SizeBox->SetRenderTranslation(loc);

		if (AttackedSideBar_Anim)
		{
			if (IsAnimationPlaying(AttackedSideBar_Anim))
			{
				StopAnimation(AttackedSideBar_Anim);
			}


			PlayAnimationForward(AttackedSideBar_Anim);
		}
	}
}
