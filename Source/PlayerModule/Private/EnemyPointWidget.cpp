// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPointWidget.h"
#include "PlayerCharacter.h"
#include "PlayerStatComponent.h"
#include "UMG.h"


void UEnemyPointWidget::NativeConstruct()
{
	Super::NativeConstruct();


}

void UEnemyPointWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SetPointOpacity();
}

void UEnemyPointWidget::StartAttacked(ABaseCharacter* Target)
{
	if (this)
	{
		TargetEnemy = Target;
		EnemyPoint_SizeBox->SetRenderOpacity(1.0f);
	}
}

void UEnemyPointWidget::SetPointOpacity()
{
	if (this && TargetEnemy)
	{
		if (UStatComponent* Estat = TargetEnemy->FindComponentByClass<UStatComponent>())
		{
			if (Estat->bDie)
			{
				TargetEnemy = nullptr;
				EnemyPoint_SizeBox->SetRenderOpacity(0.0f);
			}
		}
	}
}
