// Fill out your copyright notice in the Description page of Project Settings.


#include "DestinationWidget.h"
#include "DestinationActor.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "PlayerHUDWidget.h"
#include "CompassWidget.h"
#include "PlayerCharacter.h"
#include "UMG.h"

void UDestinationWidget::NativeConstruct()
{
	Super::NativeConstruct();


	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestinationActor::StaticClass(), desActors);
	ArraySorting();
	for (int i = 0; i < desActors.Num(); i++)
	{
		ADestinationActor* da = Cast<ADestinationActor>(desActors[i]);
		da->OnUpdateDesUIDelegate.BindUObject(this, &UDestinationWidget::PopDesActor);
	}

	nowDes = desActors[0];
	if (nowDes)
	{
		Cast<APlayerCharacter>(GetOwningPlayerPawn())->PlayerHUDWidget->CompassWidget->goalActor = nowDes;
	}
}

void UDestinationWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SetTranslation();
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
	if (nowDes)
	{
		FVector2D loc;
		if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), nowDes->GetActorLocation(), loc, true))
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, TEXT("In screen"));
			Des_Overlay->SetRenderTranslation(loc);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, TEXT("Out screen"));
		}
	}
}

void UDestinationWidget::PopDesActor(AActor* p_desact)
{
	desActors.RemoveAt(0);
	p_desact->Destroy();

	if (desActors.Num() > 0 && desActors[0])
	{
		nowDes = desActors[0];
		if (nowDes)
		{
			Cast<APlayerCharacter>(GetOwningPlayerPawn())->PlayerHUDWidget->CompassWidget->goalActor = nowDes;
		}
	}
	else
	{
		Des_Overlay->SetRenderOpacity(0.0f);
		Cast<APlayerCharacter>(GetOwningPlayerPawn())->PlayerHUDWidget->CompassWidget->goalActor = nullptr;
	}
}

