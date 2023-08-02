// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Attacked_Widget.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UAttacked_Widget : public UUserWidget
{
	GENERATED_BODY()

	FWidgetAnimationDynamicEvent OnAttackedAnimFinishEvent;
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void Init();

	void LowHp_Warning();

	void Play_Attacked_Anim(int index);

	void SetAttackedAngle();

	void StartAttacked();

	UFUNCTION(BlueprintCallable)
	void EndAttacked();

	void FindTargetEnemy();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UPlayerStatComponent* stat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* LowHP_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* LowHP_Image;


	TArray<class UOverlay*> Attacked_Overlays;
	TArray<class UImage*> Attacked_Center_Images;
	TArray<class UProgressBar*> Attacked_Side_Bars;
	TArray<class UWidgetAnimation*> Attacked_Anims;
	TArray<class AActor*> TargetEnemys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* Attacked_Overlay_0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Attacked_Center_Image_0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UProgressBar* Attacked_Side_Bar_0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* Attacked_Overlay_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Attacked_Center_Image_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UProgressBar* Attacked_Side_Bar_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* Attacked_Overlay_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Attacked_Center_Image_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UProgressBar* Attacked_Side_Bar_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* Attacked_Overlay_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Attacked_Center_Image_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UProgressBar* Attacked_Side_Bar_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* Attacked_Overlay_4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Attacked_Center_Image_4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UProgressBar* Attacked_Side_Bar_4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* LowHP_Anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Attacked_Anim_0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Attacked_Anim_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Attacked_Anim_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Attacked_Anim_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Attacked_Anim_4;

public:
	int oldEnemyIndex;

	int testEnemyNum;
	TArray<FTimerHandle>Attacked_TimerHandles;
	FTimerHandle attackedTimer_0;
	FTimerHandle attackedTimer_1;
	FTimerHandle attackedTimer_2;
	FTimerHandle attackedTimer_3;
	FTimerHandle attackedTimer_4;
};
