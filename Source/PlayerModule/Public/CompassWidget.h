// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CompassWidget.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UCompassWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void CalcPointsPos();
	void CalcGoalPos();
	void CalcEnemysPos();
	void AddEnemyPoint(class ABaseCharacter* enemy);

	bool CheckIfBehind(FVector cameraForward, FVector lookatNormal);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* Compass_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UCanvasPanel* Compass_Canvas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Points_Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* Goal_Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		TSubclassOf<class UEnemyPointWidget> EnemyPointClass;

	class UCanvasPanelSlot* Points_Slot;
	class UCanvasPanelSlot* Goal_Slot;

	TArray<ABaseCharacter*> Enemys;
	TArray<class UEnemyPointWidget*> EnemyPoints;
	class UCameraComponent* PlayerCamera;
};
