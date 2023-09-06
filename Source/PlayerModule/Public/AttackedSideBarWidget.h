// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttackedSideBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UAttackedSideBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetAttackedAngle();

	void StartAttacked(class ABaseCharacter* Target);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	class UOverlay* Attacked_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	class USizeBox* Attacked_SizeBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	class UImage* Attacked_Center_Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	class UProgressBar* Attacked_Side_Bar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* AttackedSideBar_Anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ABaseCharacter* TargetEnemy;
};
