// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AICharacterMoveComponent.generated.h"


UCLASS()
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
	UFUNCTION(BlueprintCallable)
		void SetEnemy(FName EnemyName);
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float timeDeltaTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float lerpDeltaTime;
	//에디터에서 만든 데이터 테이블을 저장할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		class UDataTable* DT_Move;
	//데이터 테이블의 변수를 저장할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float m_ChangeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float m_ParallelTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float m_SpdNomal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float m_SpdAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float m_SpdHit;

};
