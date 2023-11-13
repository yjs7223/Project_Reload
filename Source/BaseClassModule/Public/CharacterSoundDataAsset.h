// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterSoundDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class BASECLASSMODULE_API UCharacterSoundDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = aiming)
		class USoundCue* aiming_start_Cue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = aiming)
		class USoundCue* aiming_stop_Cue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = reload)
		class USoundCue* reload_cliplocked_cue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = footstep)
		TMap<TEnumAsByte<EPhysicalSurface>, USoundCue*> Footstep_Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = cover)
		class USoundCue* cover_moveC_cue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = cover)
		class USoundCue* cover_moveA_cue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = bulletImpacts)
		class USoundCue* bullet_impacts_concrete_cue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = bulletImpacts)
		class USoundCue* bullet_impacts_water_cue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = hit)
		class USoundCue* Normal_Hit_cue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = hit)
		class USoundCue* Head_Hit_cue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = hit)
		class USoundCue* Kill_cue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = hit)
		class USoundCue* missile_Hit_cue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = hit)
		class USoundCue* missile_Kill_cue;
};
