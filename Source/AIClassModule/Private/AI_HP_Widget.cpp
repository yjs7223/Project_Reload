// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_HP_Widget.h"
#include "Animation/WidgetAnimation.h"
#include "AICharacter.h"
#include "AIDog.h"
#include "AIStatComponent.h"
#include "UMG.h"


void UAI_HP_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HP_Bar)
	{
		HP_Bar->SetPercent(1.0f);
	}

	if (HP_BackBar)
	{
		HP_BackBar->SetPercent(1.0f);
	}
	HP_Overlay_s->SetRenderOpacity(1.0f);
	HP_Overlay_l->SetRenderOpacity(0.0f);



}

void UAI_HP_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CheckDistance(6000.0f))
	{
		HP_Canvas->SetRenderOpacity(.0f);
	}
	else
	{
		HP_Canvas->SetRenderOpacity(1.f);
	}

	if (stat && stat->bThreat)
	{
		SwitchVisibleWidget();
	}
	SetBackPercent(InDeltaTime);
}

void UAI_HP_Widget::SetDelegate(APawn* character)
{
	if (character)
	{
		stat = character->FindComponentByClass<UAIStatComponent>();
		if (stat)
		{
			stat->OnVisibleEnemyHPUIDelegate.BindUObject(this, &UAI_HP_Widget::SwitchVisibleWidget);
			stat->OnChangeEnemyHPUIDelegate.BindUObject(this, &UAI_HP_Widget::SetPercent);
		}
	}
}

void UAI_HP_Widget::SwitchVisibleWidget()
{
	if (HP_Overlay_s)
	{
		if (HP_Overlay_s->GetRenderOpacity() == 1.0f)
		{
			HP_Overlay_s->SetRenderOpacity(0.0f);
			HP_Overlay_l->SetRenderOpacity(1.0f);
		}
	}
}

void UAI_HP_Widget::SetPercent()
{
	if (HP_Bar)
	{
		float value = stat->curHP / stat->maxHP;
		HP_Bar->SetPercent(value);
		if (value <= 0.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(dieTimer,
				FTimerDelegate::CreateLambda([&]()
					{
						HP_Overlay_s->SetRenderOpacity(0.0f);
						HP_Overlay_l->SetRenderOpacity(0.0f);
					}
			), .5f, false);
		}
	}

}

void UAI_HP_Widget::SetBackPercent(float deltatime)
{
	if (HP_Bar && HP_BackBar)
	{
		if (HP_BackBar->Percent != HP_Bar->Percent)
		{
			float value = FMath::Lerp(HP_BackBar->Percent, HP_Bar->Percent, .02f);
			HP_BackBar->SetPercent(value);
		}
	}

}

void UAI_HP_Widget::startWarning()
{
	if (Warning_Overlay)
	{
		Warning_Overlay->SetRenderOpacity(1.0f);
	}
}

bool UAI_HP_Widget::CheckDistance(float p_dis)
{
	if (stat)
	{
		FVector ailoc = stat->GetOwner()->GetActorLocation();
		FVector pcloc = GetOwningPlayerPawn()->GetActorLocation();
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::SanitizeFloat((ailoc - pcloc).Size()));
		if ((ailoc - pcloc).Size() > p_dis)
		{
			return true;
		}
	}
	return false;
}
