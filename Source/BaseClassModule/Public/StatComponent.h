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

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Default HP Setting
	UFUNCTION(BlueprintCallable)
		void SetHP(float p_HP);

	//Recover currnetHP
	UFUNCTION(BlueprintCallable)
		void RecoverHP(float p_HP);

	//Damage process
	UFUNCTION(BlueprintCallable)
		void Attacked(float p_damage);
public:
	//maximum hp
	UPROPERTY(BlueprintReadWrite, Category = Stat)
		float maxHP;

	//now hp
	UPROPERTY(BlueprintReadWrite, Category = Stat)
		float curHP;

	//When Character is attacked   isAttacked = true
	UPROPERTY(BlueprintReadWrite, Category = Stat)
		bool isAttacked;

	//When Character's currentHP <= 0  isDie = true
	UPROPERTY(BlueprintReadWrite, Category = Stat)
		bool isDie;


};
