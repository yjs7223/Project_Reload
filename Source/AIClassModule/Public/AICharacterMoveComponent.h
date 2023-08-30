// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AICharacterMoveComponent.generated.h"
UENUM(BlueprintType)
enum class EMove : uint8
{
	Patrol 	UMETA(DisplayName = "Patrol"),
	Normal UMETA(DisplayName = "Normal"),
	Attack 	UMETA(DisplayName = "Attack"),
	Hit 	UMETA(DisplayName = "Hit")
};


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
	UFUNCTION(BlueprintCallable, Category = "AIMoveComponent")
		void MoveTimeReset();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class ABaseCharacter* owner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float Move_Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float timeDeltaTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float lerpDeltaTime;
	//�����Ϳ��� ���� ������ ���̺��� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		class UDataTable* DT_Move;
	//������ ���̺��� ������ ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float m_ChangeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float m_ParallelTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float m_HitTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float m_SpdNormal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float m_SpdAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float m_SpdHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float m_SpdPatrol;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float m_NormalMulti;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float m_AttackMulti;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float m_HitMulti;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float Time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		EMove e_move;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		class UBaseCharacterMovementComponent* movement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		class UAIInputComponent* input;
};
