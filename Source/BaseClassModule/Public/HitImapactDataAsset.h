// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HitImapactDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class BASECLASSMODULE_API UHitImapactDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* HumanHitFXNiagara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* RobotHitFXNiagara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* MetalHitFXNiagara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* RockHitFXNiagara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* MudHitFXNiagara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* WaterHitFXNiagara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* GlassHitFXNiagara;
};
