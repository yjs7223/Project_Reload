// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player_HP_Widget.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UPlayer_HP_Widget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetPercent(float percent);
	void MoveCircle(float deltatime);

	void SetShear(FRotator rot);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMaterialInstanceDynamic* DynMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* HP_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* HP_image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* HPInner_image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* OuterCircle_image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* OuterTriangle_image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* MoveCircle1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* MoveCircle2;

public:
	float moveValue1;
	float moveValue2;
};
