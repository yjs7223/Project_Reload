// Fill out your copyright notice in the Description page of Project Settings.


#include "Attacked_Widget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/WidgetAnimation.h"
#include "PlayerCharacter.h"
#include "PlayerStatComponent.h"
#include "AttackedSideBarWidget.h"
#include "UMG.h"


void UAttacked_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	if (LowHP_Overlay)
	{
		//FIntVector2 screensize;
		//GetOwningPlayer()->GetViewportSize(screensize.X, screensize.Y);
		//FVector2D size = FVector2D((double)screensize.X, (double)screensize.Y);
		//Cast<UCanvasPanelSlot>(LowHP_Overlay->Slot)->SetSize(size);
	}

	if (LowHP_Image)
	{
		LowHP_Image->SetRenderOpacity(0.0f);
	}


	if (APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn()))
	{
		if (UPlayerStatComponent* MyStatComp = Cast<UPlayerStatComponent>(MyCharacter->stat))
		{
			stat = MyCharacter->stat;
			stat->OnVisibleAttackedUIDelegate.BindUObject(this, &UAttacked_Widget::LowHp_Warning);
			MyStatComp->OnCreateAttackedUIDelegate.BindUObject(this, &UAttacked_Widget::CreateSideBarWidget);
		}
	}

}

void UAttacked_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	

}

void UAttacked_Widget::LowHp_Warning()
{
	if (this)
	{
		float value = stat->curHP / stat->maxHP;
		if (value < 0.3)
		{
			if (LowHP_Anim)
			{
				if (!IsAnimationPlaying(LowHP_Anim))
				{
					PlayAnimation(LowHP_Anim, .0f, 0);
				}
			}
		}
		else
		{
			StopAnimation(LowHP_Anim);
			LowHP_Image->SetRenderOpacity(0.0f);
		}
	}
}

void UAttacked_Widget::CreateSideBarWidget(ABaseCharacter* Target)
{
	if (AttackedSidebarWidget)
	{
		if (SideBarWidgets.Num() > 0)
		{
			for (int i = 0; i < SideBarWidgets.Num(); i++)
			{
				if (SideBarWidgets[i]->TargetEnemy == Target)
				{
					SideBarWidgets[i]->StartAttacked(Target);
					return;
				}
			}

			for (int i = 0; i < SideBarWidgets.Num(); i++)
			{
				if (SideBarWidgets[i]->TargetEnemy == nullptr)
				{
					SideBarWidgets[i]->StartAttacked(Target);
					return;
				}
			}
		}


		UAttackedSideBarWidget* m_sidebar = CreateWidget<UAttackedSideBarWidget>(GetOwningPlayer(), AttackedSidebarWidget);
		m_sidebar->StartAttacked(Target);
		m_sidebar->AddToViewport();
		SideBarWidgets.Add(m_sidebar);
	}
}



