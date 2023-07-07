// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatComponent.h"
#include "AIStatComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AICLASSMODULE_API UAIStatComponent : public UStatComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UAIStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Attacked1(float p_damage, FHitResult result) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIStat)
		AActor* IndirectCollision;


};
