// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PointLightComponent.h"
#include "LastPoint.h"
#include "AISpawner.h"
#include "ST_Spawn.h"
//#include "NiagaraComponent.h"

#include "AICharacter.generated.h"



UENUM(BlueprintType)
enum class CombatState : uint8
{
	PATROL, MOVECOVER, MOVE, ATTACK, INCOVER
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

	// 자신의 병과
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Enemy_Name type;

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

	class USkeletalMeshComponent* mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Compoenet)
		class UAICharacterMoveComponent* AIMovement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Compoenet)
		class UAIWeaponComponent* AIWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Compoenet)
		class UAIPatrolComponent* AIPatrol;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Compoenet)
		class UAISensingComponent* AISensing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Compoenet)
		class UAIStatComponent* AIStat;

	// 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* idle_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Detour;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> HP_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* HPWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIController)
		class ACharacter* player;

public:
	UFUNCTION()
		void InitWidget();
	UFUNCTION()
		void UpdateWidget();


	UFUNCTION()
		void SetDataTable(FName EnemyName);
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 순찰 대기 행동
	UFUNCTION(BlueprintCallable, Category = "Anim")
		void IdleAnim();
};
