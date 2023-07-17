// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PointLightComponent.h"
#include "AISpawner.h"
//#include "NiagaraComponent.h"
#include "AICharacter.generated.h"


UENUM(BlueprintType)
enum class CombatState : uint8
{
	PATROL, MOVECOVER, MOVE, INCOVER, ATTACK
};

UCLASS()
class AICLASSMODULE_API AAICharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AAICharacter(const FObjectInitializer& ObjectInitializer);

	// 자신의 스포너
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AAISpawner* mySpawner;
	

	//간접 공격 감지 캡슐 매쉬
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class UCapsuleComponent* CollisionMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float sup_HitRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float sup_HitHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float sup_DecInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float sup_AimPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float sup_AimDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class UDataTable* DT_Range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class UDataTable* DT_Suppression;

	// Combat State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatState")
		CombatState combat;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAICharacterMoveComponent* AIMovement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAIWeaponComponent* AIWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAIPatrolComponent* AIPatrol;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAISensingComponent* AISensing;

	// 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* idle_Montage;

public:
	UFUNCTION()
		void SetDataTable(FName EnemyName);
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 순찰 번호 검사
	UFUNCTION(BlueprintCallable, Category = "Anim")
		void IdleAnim();
};
