// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void InitWidgets();

	void CreateDamageWidget(float value, FHitResult result);

	void SetCombatWidgetVisible(bool p_visible);

	void SetAllWidgetVisible(bool p_visible);

	void CreatePauseWidget();

	void SetCorneringTranslation();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UPlayer_HP_Widget* Player_HP_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UPlayer_Ammo_Widget* Player_Ammo_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UCrosshair_Widget* Crosshair_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UAttacked_Widget* Attacked_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UInteractiveWidget* InteractiveWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UCompassWidget* CompassWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UDialogueWidget* DialogueWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UMissionWidget* MissionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UDestinationWidget* DestinationWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UCorneringWidget* CorneringWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UPlayer_Cover_Widget* CoverWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ULineNaviWidget* LineNaviWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UDamage_Widget> Damage_WidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UPauseWidget> Pause_WidgetClass;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient, EditAnywhere, BlueprintReadWrite)
		class UWidgetAnimation* CombatFadeOutAnimation;

	UPROPERTY(meta = (BindWidgetAnim), Transient, EditAnywhere, BlueprintReadWrite)
		class UWidgetAnimation* DesFadeOutAnimation;
};
