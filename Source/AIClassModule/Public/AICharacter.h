// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PointLightComponent.h"
//#include "NiagaraComponent.h"
#include "AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API AAICharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
