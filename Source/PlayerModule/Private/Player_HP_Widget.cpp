// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_HP_Widget.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "UMG.h"


void UPlayer_HP_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	mat = LoadObject<UMaterialInterface>(NULL,TEXT("Material'/Game/yjs/UI/M_Radial.M_Radial'"));
	if (mat)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, FString::Printf(TEXT("load mat")));
		DynMaterial = UMaterialInstanceDynamic::Create(mat, this);
		if (DynMaterial)
		{
			//HP_image->SetBrushFromMaterial(DynMaterial);
			//this->AddToViewport();
		}
	}

	DynMaterial->SetScalarParameterValue(FName(TEXT("Percent")), 0.5f);
	FHashedMaterialParameterInfo info(TEXT("Percent"));
	float value;
	if (DynMaterial->GetScalarParameterValue(info, value))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, FString::SanitizeFloat(value));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, FString::Printf(TEXT("fail")));
	}

}

void UPlayer_HP_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	/*DynMaterial->SetScalarParameterValue(FName(TEXT("Percent")), 1.0f);
	FHashedMaterialParameterInfo info;
	float value;
	if (DynMaterial->GetScalarParameterValue(info, value))
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::SanitizeFloat(value));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("fail")));
	}*/
	//int32 index = 0;
	//TArray<FScalarParameterValue> param = mat->ScalarParameterValues;
	////param.
	//FScalarParameterValue value;
	//value.ParameterInfo.Name = FName("Percent");
	//if (param.Find(value, index))
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::SanitizeFloat(value.ParameterValue));
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("fail")));
	//}
}

void UPlayer_HP_Widget::SetPercent(float percent)
{
	if (percent > 1.0f)
	{
		percent = 1.0f;
	}
	else if(percent < 0.0f)
	{
		percent = 0.0f;
	}

	DynMaterial->SetScalarParameterValue(FName(TEXT("Percent")), percent);
}
