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
	UCoverComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void PlayCover();
	void SettingMoveVector(FVector& vector);
	void TurnCheck(float DeltaTime);
	bool RotateSet();

protected:
	void StartCover(FHitResult& reslut);
	void StopCover();
	FHitResult CheckCoverCollision();
	void Turning();
	UFUNCTION(BlueprintCallable)
	void PlayCornering();
private:

private:
	bool m_IsCover;
	bool m_IsTurnWait;
	float m_CoverLimit;
	float m_TurnTime;

	class ACharacter* owner;
};
