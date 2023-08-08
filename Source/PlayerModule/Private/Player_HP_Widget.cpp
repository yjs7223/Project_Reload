// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_HP_Widget.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "PlayerStatComponent.h"
#include "UMG.h"

#include "Animation/WidgetAnimation.h"
#include "PlayerCharacter.h"


void UPlayer_HP_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	UMaterialInterface* mat = LoadObject<UMaterialInterface>(NULL,TEXT("Material'/Game/yjs/UI/Materials/M_RadialHP.M_RadialHP'"));
	if (mat)
	{
		HPmat = UMaterialInstanceDynamic::Create(mat, this);
		if (HPmat)
		{
			//FSlateBrush imageBrush;
			//imageBrush.ImageSize = FVector2D(30.0f, 30.0f);
			HPmat->SetScalarParameterValue(FName(TEXT("Percent")), 1.0f);
			//imageBrush.SetResourceObject(DynMaterial);
			HP_image->SetBrushFromMaterial(HPmat);
			
		}
	}

	UTexture2D* texture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/yjs/UI/Textures/HP_Texture/T_HPInner_UI.T_HPInner_UI'"));
	HPInner_image->SetBrushFromTexture(texture);


	texture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/yjs/UI/Textures/HP_Texture/T_OuterCircle.T_OuterCircle'"));
	OuterCircle_image->SetBrushFromTexture(texture);

	texture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/yjs/UI/Textures/HP_Texture/T_OuterTriangle.T_OuterTriangle'"));
	OuterTriangle_image->SetBrushFromTexture(texture);

	FSlateBrush imageBrush;
	imageBrush.ImageSize = FVector2D(32.0f, 32.0f);
	texture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/yjs/UI/Textures/HP_Texture/T_MoveCircle1.T_MoveCircle1'"));
	imageBrush.SetResourceObject(texture);
	MoveCircle1->SetBrush(imageBrush);

	imageBrush.ImageSize = FVector2D(24.0f, 24.0f);
	texture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/yjs/UI/Textures/HP_Texture/T_MoveCircle2.T_MoveCircle2'"));
	imageBrush.SetResourceObject(texture);
	MoveCircle2->SetBrush(imageBrush);

	moveValue1 = 0.0f;
	moveValue2 = 0.3;
	widgetVisibleTime = 0;
	bWidgetVisible = true;


	if(APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn()))
	{
		if (UPlayerStatComponent* MyStatComp = Cast<UPlayerStatComponent>(MyCharacter->stat))
		{
			stat = MyCharacter->stat;
			//MyStatComp->OnChangedHealthDelegate.AddRaw
			MyStatComp->OnChangedHealthDelegate.AddUObject(this, &UPlayer_HP_Widget::SetPercent);
			MyStatComp->OnVisibleHPUIDelegate.AddUObject(this, &UPlayer_HP_Widget::SetWidgetVisible);
		}
	}
}

void UPlayer_HP_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//MoveCircle(InDeltaTime);
}

void UPlayer_HP_Widget::SetPercent(float percent)
{
	if (percent > 1.0f)
	{
		percent = 1.0f;
	}
	else if (percent < 0.0f)
	{
		percent = 0.0f;
	}

	HPmat->SetScalarParameterValue(FName(TEXT("Percent")), percent);
	HP_image->SetBrushFromMaterial(HPmat);
}

void UPlayer_HP_Widget::MoveCircle(float deltatime)
{
	moveValue1 += deltatime;
	FVector2D t = FVector2D(64.0f + FMath::Sin(moveValue1) * 75.0f, 64.0f + FMath::Cos(moveValue1) * 75.0f);
	MoveCircle1->SetRenderTranslation(t);

	moveValue2 += deltatime * 1.5f;
	t = FVector2D(68.0f + FMath::Sin(moveValue2) * 73.0f, 68.0f + FMath::Cos(moveValue2) * 73.0f);
	MoveCircle2->SetRenderTranslation(t);

}

void UPlayer_HP_Widget::SetBackMat()
{
	UMaterialInterface* mat = LoadObject<UMaterialInterface>(NULL, TEXT("Material'/Game/yjs/UI/Materials/M_RadialHP_Back.M_RadialHP_Back'"));
	if (mat)
	{
		HPmat = UMaterialInstanceDynamic::Create(mat, this);
		if (HPmat)
		{
			//FSlateBrush imageBrush;
			//imageBrush.ImageSize = FVector2D(30.0f, 30.0f);
			HPmat->SetScalarParameterValue(FName(TEXT("Percent")), 1.0f);
			//imageBrush.SetResourceObject(DynMaterial);
			HP_image->SetBrushFromMaterial(HPmat);

		}
	}
}

void UPlayer_HP_Widget::SetWidgetVisible()
{
	StopAllAnimations();
	HP_Overlay->SetRenderOpacity(1.f);
	if (FadeOutAnimation)
	{
		GetWorld()->GetTimerManager().SetTimer(WTimer, FTimerDelegate::CreateLambda([&]()
			{
				if (FadeOutAnimation)
				{
					PlayAnimationForward(FadeOutAnimation);
				}
			}), 5.f, false);
	}
}

void UPlayer_HP_Widget::SetShear(FRotator rot)
{
	float s;
	if (rot.Yaw > 0)
	{
		s = FMath::Sin((180 - rot.Yaw) / 180);
	}
	else
	{
		s = FMath::Sin((rot.Yaw) / 180);
	}
	HP_Overlay->SetRenderShear(FVector2D((s * 10.0f), 0.0f));
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(rot.Yaw));
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(s));
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(s * 10.0f));
}
