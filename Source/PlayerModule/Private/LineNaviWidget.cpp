// Fill out your copyright notice in the Description page of Project Settings.


#include "LineNaviWidget.h"
#include "Kismet/GameplayStatics.h"
#include "NaviPoint.h"
#include "Rendering/DrawElements.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "CoverComponent.h"
#include "PlayerCharacter.h"

void ULineNaviWidget::NativeConstruct()
{
	Super::NativeConstruct();


	//LinePoints.Add(GetOwningPlayerPawn());
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANaviPoint::StaticClass(), LinePoints);
	LinePoints.Insert(GetOwningPlayerPawn(), 0);

	//LinePoints.Add(GetOwningPlayerPawn());
	if (LinePoints.Num() > 0)
	{
		PointLocations.SetNum(LinePoints.Num());
	}

	ArraySorting();

	if (PointLocations.Num() > 0)
	{
		FVector2D loc;
		UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), LinePoints[0]->GetActorLocation(), loc, true);
		//UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), LinePoints[0]->GetActorLocation(), loc);
		PointLocations[0] = loc;

		for (int i = 1; i < LinePoints.Num(); i++)
		{
			ANaviPoint* point = Cast<ANaviPoint>(LinePoints[i]);
			if (point && point->NaviIndex <= PointLocations.Num())
			{
				UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), point->GetActorLocation(), loc, true);
				//UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), point->GetActorLocation(), loc);
				PointLocations[point->NaviIndex] = loc;
			}
			
		}
	}

	if (APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn()))
	{
		MyCharacter->OnVisibleAllUIDelegate.AddUObject(this, &ULineNaviWidget::SetCombatWidgetVisible);

		if (UCoverComponent* myCover = MyCharacter->FindComponentByClass<UCoverComponent>())
		{
			myCover->OnCoverPointsSetDelegate.AddUObject(this, &ULineNaviWidget::SetCoverLine);
		}
	}

}

void ULineNaviWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("lnw"));

	FVector2D loc;
	FVector2D viewportsize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), LinePoints[0]->GetActorLocation(), loc, true);
	//UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), LinePoints[0]->GetActorLocation(), loc);
	PointLocations[0] = loc;

	for (int i = 1; i < LinePoints.Num(); i++)
	{
		if (IsValid(LinePoints[i]))
		{
			if (PointLocations.IsValidIndex(i))
			{
				//UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), LinePoints[i]->GetActorLocation(), loc, false);
				//UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), LinePoints[i]->GetActorLocation(), loc, true);
				
				if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), LinePoints[i]->GetActorLocation(), loc, false))
				{
					//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, loc.ToString());
					PointLocations[i] = loc;
				}
			}
		}
		else
		{
			LinePoints.RemoveAt(i);
			PointLocations.RemoveAt(i);
			RemovePrevPoint(i);
		}
	}
}

int32 ULineNaviWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	if (bWidgetVisible)
	{
		const FPaintGeometry PG = AllottedGeometry.ToPaintGeometry();
		const int32 LayerIdNew = LayerId + 1;
		//FSlateDrawElement::MakeLines(OutDrawElements, LayerIdNew, PG, PointLocations, ESlateDrawEffect::None, FLinearColor::Yellow, true, 1.0f);

		FSlateDrawElement::MakeLines(OutDrawElements, LayerIdNew, PG, CoverLocs, ESlateDrawEffect::None, FLinearColor::Blue, true, 3.0f);
	}


	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

void ULineNaviWidget::ArraySorting()
{
	for (int i = 1; i < LinePoints.Num(); i++)
	{
		ANaviPoint* point = Cast<ANaviPoint>(LinePoints[i]);
		if (point)
		{
			if (point->NaviIndex != i)
			{
				LinePoints.Swap(i, point->NaviIndex);
			}
		}
	}
}

void ULineNaviWidget::RemovePrevPoint(int index)
{
	for (int i = (index - 1); i > 0; i--)
	{
		LinePoints.RemoveAt(i);
		PointLocations.RemoveAt(i);
	}
}

void ULineNaviWidget::SetCombatWidgetVisible()
{
	if (!bWidgetVisible)
	{
		bWidgetVisible = true;

		GetWorld()->GetTimerManager().SetTimer(VisibleTimer, FTimerDelegate::CreateLambda([&]()
			{
				bWidgetVisible = false;
			}), 5.f, false);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(VisibleTimer);

		GetWorld()->GetTimerManager().SetTimer(VisibleTimer, FTimerDelegate::CreateLambda([&]()
			{
				bWidgetVisible = false;
			}), 5.f, false);
	}
}

void ULineNaviWidget::SetCoverLine(TArray<FNavPathPoint> p_CoverLocs)
{
	CoverLocs.Empty();

	if (p_CoverLocs.Num() == 0)
	{
		return;
	}

	FVector2D loc;
	for (int i = 0; i < p_CoverLocs.Num(); i++)
	{
		if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), p_CoverLocs[i].Location, loc, false))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, loc.ToString());
			CoverLocs.Add(loc);
		}
	}

	SetCombatWidgetVisible();
}
