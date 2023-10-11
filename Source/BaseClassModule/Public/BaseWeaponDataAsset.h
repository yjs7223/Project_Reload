// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BaseWeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class BASECLASSMODULE_API UBaseWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	//총기 스켈레탈메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* WeaponSkeletalMesh;

	//웨폰 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class TSubclassOf<UAnimInstance> WeaponAnim;

	//총기 발사 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
	class USoundCue* FireSound;

	//재장전 노리쇠 소리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
	class USoundCue* ReloadCliplockedSound;

	//재장전 탄창 삽입소리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
	class USoundCue* ReloadMagInSound;

	//재장전 탄창 해체소리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
	class USoundCue* ReloadMagOutSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
		class USoundCue* Bullet_Passby_Sound;

	//총구 화염 파티클
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = fx)
	class UParticleSystem* MuzzleFireParticle;

	//총알 나이아가라 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = fx)
	class UNiagaraSystem* BulletTrailFXNiagara;

	//탄흔 데칼
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMaterialInstance*> BulletHole_Decals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, class UStaticMesh*> Attachments;
};
