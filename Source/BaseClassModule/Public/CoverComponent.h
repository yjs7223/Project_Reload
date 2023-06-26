// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EPeekingState.h"
#include "ECoverShootingState.h"
#include "CoverComponent.generated.h"

using namespace UP;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASECLASSMODULE_API UCoverComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCoverComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void bindInput(class UInputComponent* PlayerInputComponent);

	UFUNCTION(Exec, BlueprintCallable)
		void CoverInputEvent();
	UFUNCTION(BlueprintCallable)
		void MoveForward(float Value);
	UFUNCTION(BlueprintCallable)
		void MoveRight(float Value);
	UFUNCTION(BlueprintCallable)
		void StartAiming();

	UFUNCTION(BlueprintCallable)
		void StopAiming();

	UFUNCTION(BlueprintCallable)
		void StartFire();
	UFUNCTION(BlueprintCallable)
		void StopFire();

	UFUNCTION(BlueprintCallable)
		FVector projectionToWay(const FVector& vec);


	UFUNCTION(BlueprintCallable)
		bool isCovering();
	UFUNCTION(BlueprintCallable)
		bool isFaceRight();
	UFUNCTION(BlueprintCallable)
		bool isCornering();
	UFUNCTION(BlueprintCallable)
		bool isAiming();
	UFUNCTION(BlueprintCallable)
		float getAimYaw();
	UFUNCTION(BlueprintCallable)
		float getAimPitch();
	UFUNCTION(BlueprintCallable)
		EPeekingState getPeekingState();
	UFUNCTION(BlueprintCallable)
		ECoverShootingState getCoverSootingState();
	UFUNCTION(BlueprintCallable)
		bool isPeeking();
	UFUNCTION(BlueprintCallable)
		bool isFire();
	UFUNCTION(BlueprintCallable)
		bool isConerWait();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float moveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float moveSpeed2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float turnLimitTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* tempMontage;

private:
	UFUNCTION()
		void OnCover();

	UFUNCTION()
		void OffCover();
	UFUNCTION()
		void StartPeeking();
	UFUNCTION()
		void StopPeeking();
	UFUNCTION()
		void Moving(float deltaTime);
	UFUNCTION()
		void FixPos(float deltaTime);
	UFUNCTION()
		void Cornering(float deltaTime);
	UFUNCTION()
		void FixAim(float deltaTime);
	UFUNCTION()
		void BeCrouch(float deltaTime);
	UFUNCTION()
		void CalculatePoint(float deltaTime);
	UFUNCTION()
		void CalculateCoverShoot(float deltaTime);

private:
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		TArray<class AActor*> mCoverWalls;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		class AActor* mCoveringWall;

	//UPROPERTY(VisibleAnywhere, Category = Gameplay)
	//	class UWeaponComponent* mWeapon;
	//UPROPERTY(VisibleAnywhere, Category = Gameplay)
	//	class UPlayerMoveComponent* mPlayerMove;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		FVector mMoveVec;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		bool mIsCovering;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		bool mIsFaceRight;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		bool mIsConering;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		bool mIsAiming;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		bool mIsFire;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		bool mIsCornerWait;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		int mIsConeringRight;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		float mIsConerTime;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		float mConeringAngle;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		float mAimYaw;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		float mAimPitch;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		FVector mTargetPos;

	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		float mConeringDeltaAngle;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		int mCurrPoint;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		FVector mUpVector;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		FVector mFrontVector;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		FVector mDownVector;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		FVector mRightVector;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		FVector mLeftVector;

	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		EPeekingState  mPeekingState;
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		ECoverShootingState mCoverShootingState;
	UPROPERTY()
		class ACharacter* Owner;
};
