// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"


UENUM(BlueprintType)
enum class EHitType: uint8
{
	None = 0	UMETA(Hidden),
	Normal		UMETA(DisplayName = "Normal"),
	Knockback	UMETA(DisplayName = "Knockback"),
	MAX			UMETA(Hidden)
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASECLASSMODULE_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDieDelegate);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FKnockbackDelegate, FVector, bool);
public:
	FKnockbackDelegate Knockback;
	UPROPERTY(BlueprintReadWrite)
	FDieDelegate diePlay;
public:	
	// Sets default values for this component's properties
	UStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	

	UFUNCTION(BlueprintCallable)
		ABaseCharacter* GetCharacter();

	//Default HP Setting
	//UFUNCTION(BlueprintCallable)
	virtual void SetHP(float p_HP);

	//Recover currnetHP
	virtual void RecoverHP(float p_HP);

	//Damage process
	/*
	virtual void Attacked(float p_damage);
	virtual void Attacked(float p_damage, FHitResult result);
	virtual void Attacked(float p_damage, class ACharacter* character);
	virtual void Attacked(FHitResult result);*/

	UFUNCTION(BlueprintCallable)
	void Attacked_BP(float p_damage = 0, class ABaseCharacter* attacker = nullptr, EHitType hittype = EHitType::Normal, FVector attackPoint = FVector::ZeroVector);
	virtual void Attacked(float p_damage = 0, class ABaseCharacter* attacker = nullptr, EHitType hittype = EHitType::Normal, FVector attackPoint = FVector::ZeroVector);
	virtual void IndirectAttacked(float p_Value);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		class ABaseCharacter* TargetEnemy;
	//maximum hp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		float maxHP;

	//now hp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		float curHP;

	//When Character is attacked   isAttacked = true
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		bool bAttacked;

	//When Character's currentHP <= 0  isDie = true
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		bool bDie;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		bool bThreat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		float HitReactionScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FVector hitNormal;

protected:
	UPROPERTY()
		class ABaseCharacter* owner;

};
