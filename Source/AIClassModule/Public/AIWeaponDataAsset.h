// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AIWeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API UAIWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* WeaponSkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* ShotSounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystem* MuzzleFireParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystem* BulletTracerParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* BulletTrailFXNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UMaterialInstance*> Decals;
};
