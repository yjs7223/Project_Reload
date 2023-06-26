// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerMoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAYERMODULE_API UPlayerMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	

	UFUNCTION(BlueprintCallable)
		void Turning(float DeltaTime);
	UFUNCTION(BlueprintCallable)
		void Turn();
	UFUNCTION(BlueprintCallable)
		void Moving(float DeltaTime);
	UFUNCTION(BlueprintCallable)
		void SetCanMove(bool canmove);
	UFUNCTION(BlueprintCallable)
		void SetCanUnCrouch(bool canCrouch);
	UFUNCTION(BlueprintCallable)
		bool IsCanMove();



private:
	UPROPERTY()
		ACharacter* owner;
	UPROPERTY()
		FVector mMoveDirect;

	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		FRotator mTargetRotate;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		bool mCanMove;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		bool mCanUnCrouch;

	UPROPERTY(EditAnywhere, Category = Gameplay)
		float turningspped;
	UPROPERTY(EditAnywhere, Category = Gameplay)
		float movespeed;
};
