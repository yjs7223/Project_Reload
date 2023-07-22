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

	void TranslateHPWidget();
	void TranslateAmmoWidget();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class APlayerCharacter* owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UPlayer_HP_Widget* WBP_PlayerHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UUserWidget* WBP_AMMO;

};
