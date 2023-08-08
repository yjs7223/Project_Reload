// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASECLASSMODULE_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

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
	UFUNCTION(BlueprintCallable)
		void RecoverHP(float p_HP);

	//Damage process
	virtual void Attacked(float p_damage);
	virtual void Attacked(float p_damage, FHitResult result);
	virtual void Attacked(float p_damage, class ABaseCharacter* character);
	virtual void Attacked(FHitResult result);

public:
	//maximum hp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		float maxHP;

	//now hp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		float curHP;

	//When Character is attacked   isAttacked = true
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		bool isAttacked;

	//When Character's currentHP <= 0  isDie = true
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		bool isDie;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		bool isThreat;

	FVector hitNormal;

protected:
	UPROPERTY()
		class ABaseCharacter* owner;

};
