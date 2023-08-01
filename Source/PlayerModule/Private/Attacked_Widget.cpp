// Fill out your copyright notice in the Description page of Project Settings.


#include "Attacked_Widget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/WidgetAnimation.h"
#include "PlayerCharacter.h"
#include "PlayerStatComponent.h"
#include "UMG.h"


void UAttacked_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	oldEnemyIndex = 0;
	testEnemyNum = 0;
	OnAttackedAnimFinishEvent.BindUFunction(this, FName(TEXT("EndAttacked")));
	//OnAttackedAnimFinishEvent.
	Init();

	if (LowHP_Overlay)
	{
		FIntVector2 screensize;
		GetOwningPlayer()->GetViewportSize(screensize.X, screensize.Y);
		FVector2D size = FVector2D((double)screensize.X, (double)screensize.Y);
		Cast<UCanvasPanelSlot>(LowHP_Overlay->Slot)->SetSize(size);
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
		}
	}

}

void UAttacked_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	SetAttackedAngle();
}

void UAttacked_Widget::Init()
{
	Attacked_Overlays.Add(Attacked_Overlay_0);
	Attacked_Overlays.Add(Attacked_Overlay_1);
	Attacked_Overlays.Add(Attacked_Overlay_2);
	Attacked_Overlays.Add(Attacked_Overlay_3);
	Attacked_Overlays.Add(Attacked_Overlay_4);

	for (int i = 0; i < Attacked_Overlays.Num(); i++)
	{
		if (Attacked_Overlays[i])
		{
			Attacked_Overlays[i]->SetRenderOpacity(0.0f);
		}
	}

	Attacked_Center_Images.Add(Attacked_Center_Image_0);
	Attacked_Center_Images.Add(Attacked_Center_Image_1);
	Attacked_Center_Images.Add(Attacked_Center_Image_2);
	Attacked_Center_Images.Add(Attacked_Center_Image_3);
	Attacked_Center_Images.Add(Attacked_Center_Image_4);

	for (int i = 0; i < Attacked_Overlays.Num(); i++)
	{
		if (Attacked_Center_Images[i])
		{
			Attacked_Center_Images[i]->SetRenderOpacity(0.0f);
		}
	}

	Attacked_Side_Bars.Add(Attacked_Side_Bar_0);
	Attacked_Side_Bars.Add(Attacked_Side_Bar_1);
	Attacked_Side_Bars.Add(Attacked_Side_Bar_2);
	Attacked_Side_Bars.Add(Attacked_Side_Bar_3);
	Attacked_Side_Bars.Add(Attacked_Side_Bar_4);

	for (int i = 0; i < Attacked_Overlays.Num(); i++)
	{
		if (Attacked_Side_Bars[i])
		{
			Attacked_Side_Bars[i]->SetPercent(0.0f);
		}
	}

	Attacked_Anims.Add(Attacked_Anim_0);
	Attacked_Anims.Add(Attacked_Anim_1);
	Attacked_Anims.Add(Attacked_Anim_2);
	Attacked_Anims.Add(Attacked_Anim_3);
	Attacked_Anims.Add(Attacked_Anim_4);

	for (int i = 0; i < Attacked_Overlays.Num(); i++)
	{
		if (Attacked_Anims[i])
		{
			BindToAnimationFinished(Attacked_Anims[i], OnAttackedAnimFinishEvent);
		}
	}

	Attacked_TimerHandles.Add(attackedTimer_0);
	Attacked_TimerHandles.Add(attackedTimer_1);
	Attacked_TimerHandles.Add(attackedTimer_2);
	Attacked_TimerHandles.Add(attackedTimer_3);
	Attacked_TimerHandles.Add(attackedTimer_4);

	for (int i = 0; i < Attacked_Overlays.Num(); i++)
	{
		TargetEnemys.Add(nullptr);
	}
}

void UAttacked_Widget::LowHp_Warning()
{
	//Play_Attacked_Anim();
	StartAttacked();
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
		LowHP_Image->SetRenderOpacity(0.0f);
		StopAnimation(LowHP_Anim);
	}
}

void UAttacked_Widget::Play_Attacked_Anim(int index)
{
	GetWorld()->GetTimerManager().ClearTimer(Attacked_TimerHandles[index]);
	StopAnimation(Attacked_Anims[index]);
	Attacked_Overlays[index]->SetRenderOpacity(1.f);
	Attacked_Center_Images[index]->SetRenderOpacity(1.f);
	Attacked_Side_Bars[index]->SetRenderOpacity(1.f);
	Attacked_Side_Bars[index]->SetPercent(1.0f);

	if (Attacked_Anims[index])
	{
		GetWorld()->GetTimerManager().SetTimer(Attacked_TimerHandles[index], FTimerDelegate::CreateLambda([=]()
			{
				if (Attacked_Anims[index])
				{
					PlayAnimationForward(Attacked_Anims[index]);
				}
			}), .5f, false);
	}
}



void UAttacked_Widget::SetAttackedAngle()
{
	for (int i = 0; i < TargetEnemys.Num(); i++)
	{
		if (TargetEnemys[i])
		{
			FVector enemyloc = TargetEnemys[i]->GetActorLocation();
			if (enemyloc != FVector::ZeroVector)
			{
				FRotator rot = UKismetMathLibrary::FindLookAtRotation(enemyloc, stat->GetOwner()->GetActorLocation());
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, rot.ToString());
				if (Attacked_Overlays[i])
				{
					float control_rot = GetOwningPlayer()->GetControlRotation().Yaw - 180;
					Attacked_Overlays[i]->SetRenderTransformAngle(rot.Yaw - control_rot);
				}

			}
		}
	}
}

void UAttacked_Widget::StartAttacked()
{
	FindTargetEnemy();
	int index = 0;
	index = TargetEnemys.Find(stat->TargetEnemy);
	if (index == INDEX_NONE)
	{
		for (int i = 0; i < TargetEnemys.Num(); i++)
		{
			if (TargetEnemys[i] == nullptr)
			{
				TargetEnemys[i] = stat->TargetEnemy;
				Play_Attacked_Anim(i);
				oldEnemyIndex = i + 1;
				if (oldEnemyIndex >= 5)
				{
					oldEnemyIndex = 0;
				}
				return;
			}
		}

		TargetEnemys[oldEnemyIndex] = stat->TargetEnemy;
		oldEnemyIndex++;
		if (oldEnemyIndex >= 5)
		{
			oldEnemyIndex = 0;
		}
	}
	else
	{
		if (index < TargetEnemys.Num())
		{
			Play_Attacked_Anim(index);
		}
	}

}

void UAttacked_Widget::EndAttacked()
{
	for (int i = 0; i < TargetEnemys.Num(); i++)
	{
		if (TargetEnemys[i])
		{
			if (Attacked_Center_Images[i]->GetRenderOpacity() == 0.0f)
			{
				TargetEnemys[i] = nullptr;
			}
		}
	}
}


//임시 적찾기(테스트용)
void UAttacked_Widget::FindTargetEnemy()
{
	TArray<AActor*> enemys;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Human"), enemys);
	if (testEnemyNum >= enemys.Num())
	{
		testEnemyNum = 0;
	}
	stat->TargetEnemy =Cast<ABaseCharacter>(enemys[testEnemyNum]);
	testEnemyNum++;
}



