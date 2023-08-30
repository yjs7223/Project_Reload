// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponData.h"
#include "AIWeaponData.generated.h"


USTRUCT(BlueprintType)
struct FAIWeaponStruct : public FWeaponRowStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIShotDataBase)
		float RecoilMax_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIShotDataBase)
		float RecoilMin_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIShotDataBase)
		float Recoil_Range;

};

UCLASS()
class AICLASSMODULE_API UAIWeaponData : public UObject
{
	GENERATED_BODY()
};