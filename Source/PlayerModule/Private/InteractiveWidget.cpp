// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveWidget.h"
#include "InteractiveActor.h"
#include "InteractiveComponent.h"
#include "PlayerStatComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "UMG.h"


void UInteractiveWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UPlayerStatComponent* statComp = GetOwningPlayerPawn()->FindComponentByClass<UPlayerStatComponent>())
	{
		statComp->OnVisibleInteractiveUIDelegate.BindUObject(this, &UInteractiveWidget::ActiveInteractWidget);
	}
}

void UInteractiveWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime); 
	

	if (InteractiveObj)
	{
		FVector2D loc;
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(InteractiveObj->GetActorLocation(), GetOwningPlayerPawn()->GetActorLocation());
		FVector objloc = InteractiveObj->GetActorLocation() + (rot.Vector() * 100.0f) + FVector::UpVector * 30.0f;
		if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), objloc, loc, false))
		{
			Interactive_Overlay->SetRenderTranslation(loc);
		}
	}
}

void UInteractiveWidget::ActiveInteractWidget(bool p_bActive, AActor* InterActor)
{
	if (p_bActive)
	{
		Interactive_Overlay->SetRenderOpacity(1.0f);
	}
	else
	{
		Interactive_Overlay->SetRenderOpacity(0.0f);
	}

	InteractiveObj = InterActor;
}

