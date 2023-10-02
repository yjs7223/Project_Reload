// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/PointLightComponent.h"
#include "AIDog.generated.h"

UCLASS()
class AICLASSMODULE_API AAIDog : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIDog();

	// Stat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
		int32 hp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
		int32 speed;

	// Explosion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
		float impulseForce;				// 폭발으로 미는 힘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
		float impactRadius;				// 폭발 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
		float Time;			
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
		float explosionTime;			// 폭발 타이머 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
		float curExplosionTime;			// 현재 폭발 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
		bool explosionEnabled;			// 폭발 활성화
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
		bool MoveStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
		float explosionStartDistance;	// 폭발 시작 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
		float explosionDamage;			// 폭발 데미지

	// 폭발 파티클
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = fx)
		class UNiagaraSystem* explosionNi;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
		UParticleSystem* explosionFX;
	// 폭발 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
		USoundBase* explosionSound;
	// 불빛 깜박임 소리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
		USoundBase* flashSound;
	
	// 불빛 오브젝트
	UPROPERTY()
		class UPointLightComponent* light;
	// 불빛 깜박임용 타이머
	float flashTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UCapsuleComponent* HidingCollider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UCoverComponent* Cover;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 폭발
	UFUNCTION(BlueprintCallable)
	void Explosion();

	// 이동
	void AIMove(const FVector Destination);

	// 빛 반짝임
	void LightFlash(float t);

	// 폭발 타이머
	void ExplosionTimer(float t);

	// 폭발하러 출발
	void StartExplosion();
};
