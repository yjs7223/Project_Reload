// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AICharacterMoveComponent.generated.h"
UENUM(BlueprintType)
enum class EMove : uint8
{
	Patrol 	UMETA(DisplayName = "Patrol"),
	Nomal UMETA(DisplayName = "Nomal"),
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		class AAICharacter* aicharacter;
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
		float m_SpdNomal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float m_SpdAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float m_SpdHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		float Time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMove)
		EMove e_move;
};
