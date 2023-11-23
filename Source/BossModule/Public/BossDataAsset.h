// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BossDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class BOSSMODULE_API UBossDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	//Skill 1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_1)
	class USoundCue* ShotSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_1)
	class UParticleSystem* MuzzleFireParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_1)
	class UNiagaraSystem* BulletTrailFXNiagara;

	//Skill 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_2)
	class USoundCue* LockOnSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_2)
	class USoundCue* MissileShotSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_2)
	class USoundCue* MissileExplodeSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_2)
	class UParticleSystem* MissileMuzzleFireParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_2)
	class UParticleSystem* MissileExplodeParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_2)
	class UNiagaraSystem* MissileTrailFXNiagara;

	//Skill 3
	/**/
	//Skill 4
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_4)
	class USoundCue* FlyingDroneSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_4)
	class USoundCue* DroneShotSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_4)
	class USoundCue* DroneExplodeSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_4)
	class UParticleSystem* DroneExplodeParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_4)
	class UNiagaraSystem* DroneTrailFXNiagara;

	//Special Move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Special_Move)
	class USoundCue* FlyingBossSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Special_Move)
	class USoundCue* BurnSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Special_Move)
	class UParticleSystem* BurnParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Special_Move)
	class USoundWave* BurnSoundWav;
};
