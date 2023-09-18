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

	//�ѱ� ���̷�Ż�޽�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* WeaponSkeletalMesh;

	//���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class TSubclassOf<UAnimInstance> WeaponAnim;

	//�ѱ� �߻� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
		class USoundCue* FireSound;

	//������ �븮�� �Ҹ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
		class USoundCue* ReloadCliplockedSound;

	//������ źâ ���ԼҸ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
		class USoundCue* ReloadMagInSound;

	//������ źâ ��ü�Ҹ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
		class USoundCue* ReloadMagOutSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
		class USoundCue* Bullet_Passby_Sound;

	//�ѱ� ȭ�� ��ƼŬ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = fx)
		class UParticleSystem* MuzzleFireParticle;

	//�Ѿ� ���̾ư�������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = fx)
		class UNiagaraSystem* BulletTrailFXNiagara;

	//ź�� ��Į
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class UMaterialInstance*> BulletHole_Decals;
};
