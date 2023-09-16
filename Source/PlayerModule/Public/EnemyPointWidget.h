// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyPointWidget.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UEnemyPointWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void StartAttacked(class ABaseCharacter* Target);

	void SetPointOpacity();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ABaseCharacter* TargetEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class USizeBox* EnemyPoint_SizeBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
		class UImage* EnemyPoint_Image;
};
