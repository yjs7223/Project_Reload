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

public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void LowHp_Warning();

	void CreateSideBarWidget(class ABaseCharacter* Target);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPlayerStatComponent* stat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UAttackedSideBarWidget> AttackedSidebarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class UAttackedSideBarWidget*> SideBarWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UCanvasPanel* Attacked_Canvas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UOverlay* LowHP_Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* LowHP_Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* LowHP_Anim;

};
