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

	// 최대 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float boss_MaxDmg;
	// 최소 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float boss_MinDmg;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 공격
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void TakeDamage(AActor* p_TargetActor, float p_Damage);
};
