// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponData.h"
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
struct FPlayerWeaponStruct : public FWeaponRowStruct
{
	GENERATED_USTRUCT_BODY()
public:

	//�ִ� ��ź��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmo_num;

	//��弦 ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Head_Magnification;

	//���ιݵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float max_Horizontal_Recoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float min_Horizontal_Recoil;

	//���ιݵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float max_vertical_Recoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float min_vertical_Recoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxPitchRecoilValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimingRecoilValue;

	//ź���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float spread_Power;
	//총 길이
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WeaponDistance;
};
