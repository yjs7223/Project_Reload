// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"
#include "WeaponAnimDataAsset.generated.h"

/**
 * 
 */
USTRUCT(Atomic, BlueprintType)
struct FWeaponAnimation : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	bool IsVaild();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Aiming;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Aiming_Shooting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Shooting_Base;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* Shooting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* Reload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAimOffsetBlendSpace* AimOffset;
};