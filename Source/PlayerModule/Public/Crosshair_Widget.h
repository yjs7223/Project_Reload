// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetVisibleInterface.h"
#include "Blueprint/UserWidget.h"
#include "Crosshair_Widget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PLAYERMODULE_API UCrosshair_Widget : public UUserWidget, public IWidgetVisibleInterface
{
	GENERATED_BODY()
	
public:
	UCrosshair_Widget(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeDestruct() override;

	void UpdateCrosshair(float DeltaTime);

	void SetCrosshairTranslation();

	void CalcOffset(float spreadpower);

	bool CalcAlphaValue(float DeltaTime);

	void MoveDot();

	void CheckDie();

	void SetAmmoImage();

	void PlayReloadAnim();

	UFUNCTION(BlueprintCallable)
	void StopReloadAnim();

	void Blocking(bool bBlock);

	virtual void SetCombatWidgetVisible() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPlayerWeaponComponent* weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* Crosshair_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* Reload_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* Hit_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* Kill_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* NotFire_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Up_Cross_image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Down_Cross_image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Left_Cross_image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Right_Cross_image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Up_Cross_image_s;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Down_Cross_image_s;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Left_Cross_image_s;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Right_Cross_image_s;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Dot_image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Hit_Image_NW;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Hit_Image_NE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Hit_Image_SW;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Hit_Image_SE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Kill_Image_NW;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Kill_Image_NE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Kill_Image_SW;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Kill_Image_SE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Cross_Ammo_Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMaterialInstanceDynamic* AmmoMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* FadeOutAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* ReloadAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* HitAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* KillAnim;

public:
	float m_alpha;
	float m_offset;
	float m_offset_s;

	float m_hitTime;

	bool bWidgetVisible;
	float widgetVisibleTime;
	FTimerHandle VisibleTimer;
	FTimerHandle DotTimer;
	FTimerHandle HitTimer;
	FTimerHandle KillTimer;
	FTimerHandle ReloadTimer;
};
