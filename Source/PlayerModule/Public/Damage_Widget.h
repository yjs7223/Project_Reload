// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Damage_Widget.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UDamage_Widget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetDamageText(float value, FHitResult result);
	void SetWidgetLocation(float InDeltaTime);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* Damage_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UTextBlock* Damage_Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* DamageAnim;

public:
	FVector hitLocation;
	FVector2D damageLocation;
	FVector2D addValue;
	FTimerHandle DamageTimer;
};
