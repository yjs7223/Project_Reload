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
	DECLARE_MULTICAST_DELEGATE(FStartCoverEventDelegate);
	DECLARE_MULTICAST_DELEGATE(FEndCoverEventDelegate);
	DECLARE_MULTICAST_DELEGATE_OneParam(FPlayerCharacterTickDelegate, float /*DeltaTime*/);
	DECLARE_MULTICAST_DELEGATE_OneParam(FCoverCharacterTickDelegate, float /*DeltaTime*/);
	DECLARE_MULTICAST_DELEGATE_OneParam(FCoverPointsSetDelegate, TArray<FNavPathPoint>);
	DECLARE_DELEGATE_TwoParams(FOnVisibleCorneringWidget, bool /*bvisible*/, bool /*bright*/);
	DECLARE_DELEGATE_OneParam(FOnSetPercentCorneringWidget, float /*percent*/);
	DECLARE_DELEGATE_OneParam(FOnVisibleCoverWidget, FVector);
	DECLARE_DELEGATE_OneParam(FOnFaceRightCoverWidget, bool);

public:
	/** 엄폐시작 몽타주재생 델리게이트입니다 */
	FStartCoverEventDelegate PlayMontageStartCover;
	/** 엄폐해제 몽타주재생 델리게이트입니다 */
	FEndCoverEventDelegate PlayMontageEndCover;
	/** 플레이어캐릭터의 엄폐 틱델리게이트 입니다 */
	FPlayerCharacterTickDelegate PlayerCharacterTick;
	/** 엄폐캐릭터의 엄폐 틱델리게이트 입니다 */
	FCoverCharacterTickDelegate CoverCharacterTick;
	/** UI 커버포인트 세팅 델리게이트 입니다 */
	FCoverPointsSetDelegate OnCoverPointsSetDelegate;
	/** UI 코너링 위젯 델리게이트 입니다 */
	FOnVisibleCorneringWidget OnVisibleCorneringWidget;
	/** 코너링 퍼센트 위젯UI 델리게이트 입니다 */
	FOnSetPercentCorneringWidget OnSetPercentCorneringWidget;

	FOnVisibleCoverWidget OnVisibleCoverWidget;

	FOnFaceRightCoverWidget OnFaceRightCoverWidget;
public:
	UCoverComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** 엄폐버튼을 눌렀을때 실행합니다 */
	void PlayCover();
	/**
	* 이동방향을 세팅해줍니다
	* @param vector 세팅될 이동벡터 입니다
	*/
	void SettingMoveVector(OUT FVector& vector);
	/** 엄폐용으로 에임오프셋을 세팅해줍니다 */
	void AimSetting(float DeltaTime);
	/** 엄폐중에 로테이트를 세팅해줍니다*/
	void RotateSet(float DeltaTime);
	/**
	* 엄폐 가능한 점을 구합니다
	* @return 커버가능한점을 반환합니다 FVector::ZeroVector면 실패입니다
	*/
	void SettingCoverPoint(float DeltaTime);
	FVector CalculateCoverPoint(float DeltaTime);
	/**
	* 엄폐 가능한 점을 세팅합니다
	* @param point 엄폐할 지점 입니다
	*/
	void SetCanCoverPoint(FVector point);
	/** 엄폐중이면 참을 반환합니다 */
	bool IsCover();
	/** 다음 엄폐로 넘어가는중이면 참입니다*/
	bool IsNextCover();
	/** 코너링대기중이면 참을 반환합니다 */
	bool IsTurnWait();
	/** m_FaceRight값을 반환 합니다 */
	UFUNCTION(BlueprintCallable)
	float FaceRight();
	UFUNCTION(BlueprintCallable)
	/** 오른손잡이면 참을 반환합니다 */
	bool IsFaceRight();
	/** 캐릭터의 좌우방향을 바꿉니다 */
	void SetIsFaceRight(bool faceRight);
	/** 코너링중이면 참을 반환합니다 */
	bool IsCornering();
	/** 피킹중이면 참을 반환합니다 */
	bool IsPeeking();
	UFUNCTION(BlueprintCallable)
	/** 피킹중임을 세팅해 줍니다*/
	void SetIsPeeking(bool isPeeking);
	/** 엄폐사격상태를 계산합니다 */
	void CalculateCoverShoot(float DeltaTime);
	/** 엄폐사격상태를 반환합니다 */
	ECoverShootingState getCoverSootingState();
	
	/** 피킹상태를 반환합니다 */
	UFUNCTION(BlueprintCallable)
	EPeekingState getPeekingState();
	/** 엄폐가능 위치를 반환합니다 */
	FVector getCanCoverPoint();
	/** 엄폐가능 위치에서 노말값을 반환합니다 */
	FVector GetPointNormal();

	/** 피킹을 시작합니다 */
	void StartPeeking();
	/** 피킹상태를 계산합니다 */
	void peekingCheck(FRotator& aimOffset);
	/** 피킹중단합니다 */
	void StopPeeking();
	/** 엄폐중인벽을 반환합니다 */
	AActor* GetCoverWall();
	UFUNCTION(BlueprintCallable)
	/** 엄폐를 중단합니다 */
	void StopCover();

	/** AI용 엄폐시작 입니다 */
	UFUNCTION(BlueprintCallable)
	bool StartAICover();

	/** 엄폐시 수그려야하는지 반환합니다 */
	bool isMustCrouch();
protected:
	/** 코너링을 체크합니다 */
	void CornenringCheck(float DeltaTime);
	/** 엄폐경로를 계산합니다 */
	TArray<FNavPathPoint> CalculCoverPath();
	/** 엄폐경로를 UI에 세팅합니다 */
	void SettingCoverPath(float DeltaTime);
	/** 엄폐를 시작합니다 */
	bool StartCover();
	/** 앞에 엄폐벽이있는지 체크합니다 */
	void CheckCoverCollision(OUT FHitResult& result);
	/** 코너링을 시작합니다 */
	void StartCornering();
	/** 코너링을 중단합니다 */
	void StopCornering(float DeltaTim);
	/** 코너링중에 실행합니다 */
	void PlayingCornering(float DeltaTim);
	/** 엄폐시 수그려야하면 수그립니다 */
	void BeCrouch(float deltaTime);
	/** 엄폐이동이 끝나면 호출됩니다*/
	void AIMoveCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result);

protected:
	/** 엄폐벽을 체크할 트레이스채널 입니다 */
	static const ECollisionChannel traceChanel = ECC_GameTraceChannel1;
	/** 엄폐오브젝트타입채널 입니다 */
	static const ECollisionChannel coverWallType = ECC_GameTraceChannel2;

private:

	class UBaseCharacterMovementComponent* m_Movement;
	class UBaseInputComponent* m_Input;
	struct FInputData* m_Inputdata;
	class UWeaponComponent* m_Weapon;
	class UCapsuleComponent* capsule;
	class AActor* m_CoverWall;
	class UPathFollowingComponent* m_PathFollowingComp;
	enum class ECoverShootingState mCoverShootingState;
	UPROPERTY(VisibleInstanceOnly, meta = (AllowPrivateAccess = true))
	EPeekingState m_PeekingState;
	EPeekingState m_PeekingInitialState;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Editor)
	bool m_IsCover;


private:
	bool m_IsCorneringWait;
	bool m_IsCornering;
	bool m_IsPeeking;
	bool m_IsNextCover;
	float m_FaceRight;
	float m_CurrentCorneringWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, DisplayName = "CorneringWaitTime"))
	float m_CorneringWaitTime;
	FVector m_Turnlookpoint;
	FVector m_CanCoverPoint;
	FVector m_CanCoverPointNormal;
	class ACharacter* owner;

	bool m_IsWillPosSetting;

};
