// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetVisibleInterface.h"
#include "Player_HP_Widget.generated.h"


/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UPlayer_HP_Widget : public UUserWidget, public IWidgetVisibleInterface
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeDestruct() override;


	UFUNCTION(BlueprintCallable)
	void SetPercent(float percent);
	void MoveCircle(float deltatime);
	void SetBackMat();

	void SetWidgetVisible() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPlayerStatComponent* stat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMaterialInstanceDynamic* HPmat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* HP_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* HP_image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UProgressBar* HP_Bar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* OuterCircle_image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* OuterTriangle_image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* MoveCircle1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* MoveCircle2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		FTimerHandle WTimer;

	UPROPERTY(meta = (BindWidgetAnim), Transient, EditAnywhere, BlueprintReadWrite)
		class UWidgetAnimation* FadeOutAnimation;

public:
	bool bWidgetVisible;
	bool bHpBack;
	float widgetVisibleTime;
	float moveValue1;
	float moveValue2;
};
