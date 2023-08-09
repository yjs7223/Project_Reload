// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacterMovementComponent.generated.h"



UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None			UMETA(Hidden),

	CMOVE_Runing			UMETA(DisplayName = "Run"),

	CMOVE_MAX			UMETA(Hidden)
};

ENUM_CLASS_FLAGS(ECustomMovementMode)

/**
 * 
 */
UCLASS()
class BASECLASSMODULE_API UBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
		
public:
	bool isRuning() const;
public:
	/** Check if pawn is falling */
	virtual bool CheckFall(const FFindFloorResult& OldFloor, const FHitResult& Hit, const FVector& Delta, const FVector& OldLocation, float remainingTime, float timeTick, int32 Iterations, bool bMustJump);

	virtual float GetMaxSpeed() const override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void PhysCustom(float deltaTime, int32 Iterations);

public:

	UPROPERTY(Category = "Character Movement: CMOVE_Runing", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float MaxRuningSpeed = 600.0f;
protected:

private:
	

};
