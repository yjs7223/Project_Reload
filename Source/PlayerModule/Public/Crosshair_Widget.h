// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Crosshair_Widget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PLAYERMODULE_API UCrosshair_Widget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCrosshair_Widget(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void UpdateCrosshair(float DeltaTime);

	void SetCrosshairTranslation();

	void CalcOffset(float spreadpower);

	bool CalcAlphaValue(float DeltaTime);

	void CheckHit(float delta);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPlayerWeaponComponent* weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* Crosshair_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Up_Cross_image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Down_Cross_image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Left_Cross_image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Right_Cross_image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Dot_image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Hit_image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* HeadHit_image;

public:
	float m_alpha;
	float m_offset;

	float m_hitTime;
};
