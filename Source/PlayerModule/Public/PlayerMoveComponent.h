// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Input.h"
#include "PlayerMoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAYERMODULE_API UPlayerMoveComponent : public UActorComponent, public IInput
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//function
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void bindInput(class UInputComponent* PlayerInputComponent) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Move)
		float moveSpeed;



};
