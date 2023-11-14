// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeOutWidget.h"
#include "PlayerStatComponent.h"
#include "Animation/WidgetAnimation.h"
#include "UMG.h"

void UTimeOutWidget::NativeConstruct()
{
	Super::NativeConstruct();


}

void UTimeOutWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


}

void UTimeOutWidget::StartCount(int p_timeCount)
{
	maxCount = p_timeCount;
	leftCount = p_timeCount;

	CountTime();
	TimeOut_Overlay->SetRenderOpacity(1.0f);

	GetOwningPlayer()->GetWorldTimerManager().SetTimer(timeOutHandle, this, &UTimeOutWidget::CountTime, 1.0f, true);
}

void UTimeOutWidget::CountTime()
{
	leftCount--;

	int m = leftCount / 60;
	int s = leftCount % 60;
	FText t;
	if (s < 10)
	{
		t = FText::FromString("0" + FString::FromInt(m) + " : 0" + FString::FromInt(s));
	}
	else
	{
		t = FText::FromString("0" + FString::FromInt(m) + " : " + FString::FromInt(s));
	}
	
	TimeOut_Text->SetText(t);

	if (leftCount > 10)
	{
		PlayAnimationForward(CountAnim);
	}
	else
	{
		PlayAnimationForward(FastCountAnim);
	}

	if (leftCount == 0)
	{
		if (UPlayerStatComponent* mySyat = GetOwningPlayerPawn()->FindComponentByClass<UPlayerStatComponent>())
		{
			mySyat->diePlay.Broadcast();
			GetOwningPlayer()->GetWorldTimerManager().ClearTimer(timeOutHandle);
		}
	}
}
