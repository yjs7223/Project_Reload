// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AICharacterMoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AICLASSMODULE_API UAICharacterMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAICharacterMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float Move_Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		bool AI_Move;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		bool Move_Normal; //기본 이동시 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		bool Move_Attack; //ai가 공격시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		bool Move_Hit; //ai가 피격시
};
