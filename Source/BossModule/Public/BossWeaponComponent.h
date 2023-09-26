// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOSSMODULE_API UBossWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossWeaponComponent();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BossWeaponSetting)
		class ABaseCharacter* owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossWeaponSetting")
		class USkeletalMeshComponent* WeaponMesh;

		class USkeletalMeshComponent* playerMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossWeaponSetting")
		class ACharacter* player;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossWeaponSetting")
		float boss_MaxDmg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossWeaponSetting")
		float boss_MinDmg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossWeaponSetting")
		float recoil_Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossWeaponSetting")
		float shot_MaxRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossWeaponSetting")
		float recoilMax_Radius;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossWeaponSetting")
		float recoilMin_Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossWeaponSetting")
		float recoil_Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossWeaponSetting")
		int shot_MaxCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FHitResult m_result;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ����
	UFUNCTION(BlueprintCallable, Category = "BossAttack")
		void TakeDamage(AActor* p_TargetActor, float p_Damage);

	UFUNCTION(BlueprintCallable, Category = "BossAttack")
		FRotator Fire(FVector Start);
};
