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
	Stun		UMETA(DisplayName = "Stun"),
	MAX			UMETA(Hidden)
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BASECLASSMODULE_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDieDelegate);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FKnockbackDelegate, FVector, bool);
public:
	FKnockbackDelegate Knockback;
	UPROPERTY(BlueprintCallable, BlueprintReadWrite)
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
	virtual void SetHP(float p_HP);

	//Recover currnetHP
	virtual void RecoverHP(float p_HP);

	//Damage process
	UFUNCTION(BlueprintCallable)
	void Attacked_BP(float p_damage = 0, class ABaseCharacter* attacker = nullptr, EHitType hittype = EHitType::Normal, FVector attackPoint = FVector::ZeroVector);
	virtual void Attacked(float p_damage = 0, class ABaseCharacter* attacker = nullptr, EHitType hittype = EHitType::Normal, FVector attackPoint = FVector::ZeroVector);
	virtual void IndirectAttacked(float p_Value);

	bool IsStun() { return bIsStun; }
	bool IsKnockback() { return bIsKnockback; }
	void SetKnockback(bool p_Knockback);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	class ABaseCharacter* TargetEnemy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Input"))
	TObjectPtr<class UBaseInputComponent> m_Input;
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
	class ABaseCharacter* owner;

	bool bIsStun;
	bool bIsKnockback;
};
