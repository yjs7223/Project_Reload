// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerMovable.h"
#include "PlayerMoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAYERMODULE_API UPlayerMoveComponent : public UActorComponent, public IPlayerMovable
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
		virtual	void SetCanMove(bool canmove) override;
	UFUNCTION(BlueprintCallable)
		void SetCanUnCrouch(bool canCrouch);
	UFUNCTION(BlueprintCallable)
		bool IsCanMove();


private:
	ACharacter* owner;
	FVector mMoveDirect;
	class UCoverComponent* m_CoverComp;
	class UActorComponent* m_PakurComp;
	struct FInputData* m_Inputdata;
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
