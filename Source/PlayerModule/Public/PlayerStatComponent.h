// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatComponent.h"
#include "PlayerStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangedHeathDelegate, float);
//DECLARE_DELEGATE_OneParam(FOnChangedHeathDelegate, float);
DECLARE_MULTICAST_DELEGATE(FOnVisibleHPUIDelegate);
DECLARE_DELEGATE(FOnVisibleAttackedUIDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateAttackedUIDelegate, class ABaseCharacter*);
DECLARE_DELEGATE_TwoParams(FOnVisibleInteractiveUIDelegate, bool, AActor*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSetInteractUIDelegate, FVector);

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UPlayerStatComponent : public UStatComponent
{
	GENERATED_BODY()

public:
	UPlayerStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetHP(float p_HP) override;

	UFUNCTION(BlueprintCallable)
	void RecoverHP(float p_HP) override;

	UFUNCTION(BlueprintCallable)
	void Attacked(float p_damage = 0, class ABaseCharacter* attacker = nullptr, EHitType hittype = EHitType::Normal, FVector attackPoint = FVector::ZeroVector) override;

	UFUNCTION(BlueprintCallable)
	void CheckInteractiveObj();
	
	UFUNCTION(BlueprintCallable)
	void Interacting();

	UFUNCTION(BlueprintCallable)
	void Revive();

public:
	FOnChangedHeathDelegate OnChangedHealthDelegate;
	FOnVisibleHPUIDelegate OnVisibleHPUIDelegate;
	FOnVisibleAttackedUIDelegate OnVisibleAttackedUIDelegate;
	FOnCreateAttackedUIDelegate OnCreateAttackedUIDelegate;
	FOnVisibleInteractiveUIDelegate OnVisibleInteractiveUIDelegate;
	FOnSetInteractUIDelegate OnSetInteractUIDelegate;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UMatineeCameraShake> AttackedCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* InteractActor;
	
	FTimerHandle HpRecoverTimer;
	FTimerHandle StandbyTimer;
};
