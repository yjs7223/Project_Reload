// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetVisibleInterface.h"
#include "Player_Ammo_Widget.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UPlayer_Ammo_Widget : public UUserWidget, public IWidgetVisibleInterface
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void UpdateAmmo();

	virtual void SetWidgetVisible() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPlayerWeaponComponent* weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* Ammo_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Ammo_Middle_image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Ammo_Edge_image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Ammo_Infinite_image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UTextBlock* Max_Ammo_Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UTextBlock* Cur_Ammo_Text;

public:
	bool bWidgetVisible;
	float widgetVisibleTime;
};
