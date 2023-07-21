// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_HP_Widget.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "UMG.h"


void UPlayer_HP_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	UMaterialInterface* mat = LoadObject<UMaterialInterface>(NULL,TEXT("Material'/Game/yjs/UI/Materials/M_RadialHP.M_RadialHP'"));
	if (mat)
	{
		DynMaterial = UMaterialInstanceDynamic::Create(mat, this);
		if (DynMaterial)
		{
			//FSlateBrush imageBrush;
			//imageBrush.ImageSize = FVector2D(30.0f, 30.0f);
			DynMaterial->SetScalarParameterValue(FName(TEXT("Percent")), 1.0f);
			//imageBrush.SetResourceObject(DynMaterial);
			HP_image->SetBrushFromMaterial(DynMaterial);
			
		}
	}

	UTexture2D* texture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/yjs/UI/Textures/HP_Texture/T_HPInner_UI.T_HPInner_UI'"));
	HPInner_image->SetBrushFromTexture(texture);


	texture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/yjs/UI/Textures/HP_Texture/T_OuterCircle.T_OuterCircle'"));
	OuterCircle_image->SetBrushFromTexture(texture);

	texture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/yjs/UI/Textures/HP_Texture/T_OuterTriangle.T_OuterTriangle'"));
	OuterTriangle_image->SetBrushFromTexture(texture);

	FSlateBrush imageBrush;
	imageBrush.ImageSize = FVector2D(48.0f, 48.0f);
	texture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/yjs/UI/Textures/HP_Texture/T_MoveCircle1.T_MoveCircle1'"));
	imageBrush.SetResourceObject(texture);
	MoveCircle1->SetBrush(imageBrush);

	imageBrush.ImageSize = FVector2D(32.0f, 32.0f);
	texture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/yjs/UI/Textures/HP_Texture/T_MoveCircle2.T_MoveCircle2'"));
	imageBrush.SetResourceObject(texture);
	MoveCircle2->SetBrush(imageBrush);

	moveValue1 = 0;
	moveValue2 = 5.0f;
}

void UPlayer_HP_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("hi"));
}

void UPlayer_HP_Widget::SetPercent(float percent)
{
	/*FHashedMaterialParameterInfo info(TEXT("Percent"));
	float value;
	if (DynMaterial->GetScalarParameterValue(info, value))
	{
		value -= percent;
		if (value > 1.0f)
		{
			value = 1.0f;
		}
		else if (value < 0.0f) 
		{
			value = 0.0f;
		}
		DynMaterial->SetScalarParameterValue(FName(TEXT("Percent")), value);
	}*/

	if (percent > 1.0f)
	{
		percent = 1.0f;
	}
	else if (percent < 0.0f)
	{
		percent = 0.0f;
	}

	DynMaterial->SetScalarParameterValue(FName(TEXT("Percent")), percent);
	HP_image->SetBrushFromMaterial(DynMaterial);
}

void UPlayer_HP_Widget::MoveCircle(float deltatime)
{
	moveValue1 += deltatime;
	FVector2D t = FVector2D(76.0f + FMath::Sin(moveValue1) * 95.0f, 76.0f + FMath::Cos(moveValue1) * 95.0f);
	MoveCircle1->SetRenderTranslation(t);

	moveValue2 += deltatime;
	t = FVector2D(84.0f + FMath::Sin(moveValue2) * 80.0f, 84.0f + FMath::Cos(moveValue2) * 80.0f);
	MoveCircle2->SetRenderTranslation(t);

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
