// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PointLightComponent.h"
#include "AISpawner.h"
//#include "NiagaraComponent.h"
#include "AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API AAICharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AAICharacter();

	// 자신의 스포너
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AAISpawner* mySpawner;
	

	//간접 공격 감지 캡슐 매쉬
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class UCapsuleComponent* CollisionMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float HitRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		float HitHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class UDataTable* DT_Range;
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

public:
	UFUNCTION()
		void SetDataTable(FName EnemyName);
};
