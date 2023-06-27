// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "PlayerWeaponData.generated.h"

UCLASS()
class PLAYERMODULE_API APlayerWeaponData : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerWeaponData();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};  

USTRUCT(Atomic, BlueprintType)
struct FPlayerweaponStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 bullet_Num;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float max_Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float min_Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float max_H_Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float min_H_Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float max_Horizontal_Recoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float min_Horizontal_Recoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float max_vertical_Recoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float min_vertical_Recoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fire_Rate;
};
