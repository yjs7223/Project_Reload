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
	void AimSetting(float DeltaTime);
	void RotateSet(float DeltaTime);
	/*
	* 커버 가능한 점을 구합니다
	* @param DeltaTime 업데이트에서 호출됨을 뜻합니다
	* @return 커버가능한점을 반환합니다 FVector::ZeroVector면 실패입니다
	*/
	FVector CalculateCoverPoint(float DeltaTime);

	bool IsCover();
	bool IsTurnWait();
	UFUNCTION(BlueprintCallable)
	float FaceRight();
	bool IsCornering();
	bool isPeeking();

	void CalculateCoverShoot();
	ECoverShootingState getCoverSootingState();
	EPeekingState getPeekingState();


	UFUNCTION(BlueprintCallable)
	void StopCover();
protected:
	void TurnCheck(float DeltaTime);
	bool StartCover();

	void CheckCoverCollision(OUT FHitResult& result);
	void PlayCornering();
	void StopCornering(float DeltaTim);
	void PlayingCornering(float DeltaTim);
	void BeCrouch(float deltaTime);
	void StartPeeking();
	void StopPeeking();
	void AIMoveCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result);

private:
	//엄폐벽을 체크할 트레이스채널 입니다
	static const ECollisionChannel traceChanel = ECC_GameTraceChannel1;
	//엄폐오브젝트타입채널 입니다
	static const ECollisionChannel coverWallType = ECC_GameTraceChannel2;
	

private:
	//
	class UCharacterMovementComponent* m_Movement;
	struct FInputData* m_Inputdata;
	class UWeaponComponent* m_Weapon;
	class UCapsuleComponent* capsule;
	class AActor* m_CoverWall;
	class UPathFollowingComponent* m_PathFollowingComp;
	enum class ECoverShootingState mCoverShootingState;
	enum class EPeekingState mPeekingState;
	bool m_IsCover;
	bool m_IsTurnWait;
	bool m_IsCornering;
	float m_FaceRight;
	float m_TurnTime;
	FVector m_Turnlookpoint;
	FVector m_CanCoverPoint;
	class ACharacter* owner;

	bool m_IsWillPosSetting;
};
