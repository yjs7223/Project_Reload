// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* WeaponSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class TSubclassOf<UAnimInstance> weaponAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
		class USoundCue* ShotSound;

	//ÀçÀåÀü ³ë¸®¼è ¼Ò¸®
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
		class USoundCue* ReloadCliplockedSound;

	//ÀçÀåÀü ÅºÃ¢ »ðÀÔ¼Ò¸®
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
		class USoundCue* ReloadMagInSound;

	//ÀçÀåÀü ÅºÃ¢ ÇØÃ¼¼Ò¸®
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
		class USoundCue* ReloadMagOutSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = fx)
		class UParticleSystem* MuzzleFireParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = fx)
		class UNiagaraSystem* BulletTrailFXNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UMaterialInstance*> Decals;
};
