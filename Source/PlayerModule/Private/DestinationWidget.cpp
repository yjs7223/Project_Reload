// Fill out your copyright notice in the Description page of Project Settings.


#include "DestinationWidget.h"
#include "DestinationActor.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "UMG.h"

void UDestinationWidget::NativeConstruct()
{
	Super::NativeConstruct();


	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestinationActor::StaticClass(), desActors);
	ArraySorting();

	nowDes = desActors[0];
	
}

void UDestinationWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (nowDes)
	{
		FVector2D loc;
		if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), nowDes->GetActorLocation(), loc, false))
		{
			Des_Overlay->SetRenderTranslation(loc);
		}
	}
}

void UDestinationWidget::ArraySorting()
{
	for (int i = 0; i < desActors.Num(); i++)
	{
		ADestinationActor* dAct = Cast<ADestinationActor>(desActors[i]);
		if (dAct)
		{
			if (dAct->desIndex != i)
			{
				desActors.Swap(i, dAct->desIndex);
			}
		}
	}
}

void UDestinationWidget::SetTranslation()
{
	if (nowDes != desActors[0])
	{
		desActors.RemoveAt(0);
		nowDes = desActors[0];

	}

}
