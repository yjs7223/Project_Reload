// Fill out your copyright notice in the Description page of Project Settings.


#include "AIDog.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "StatComponent.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>

// Sets default values
AAIDog::AAIDog()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionFX(TEXT("ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_B.P_Explosion_Big_B'"));
	if (ExplosionFX.Succeeded())
	{
		explosionFX = ExplosionFX.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase> ExplosionSound(TEXT("SoundCue'/Game/SGJ/Explosion_Dog_Cue.Explosion_Dog_Cue'"));
	if (ExplosionSound.Succeeded())
	{
		explosionSound = ExplosionSound.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase> FlashSound(TEXT("SoundCue'/Game/SGJ/Explosion_Timer_Cue.Explosion_Timer_Cue'"));
	if (FlashSound.Succeeded())
	{
		flashSound = FlashSound.Object;
	}
}

void AAIDog::StartExplosion()
{
	FVector pLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	float const distance = FVector::Dist(pLocation, GetActorLocation());

	AIMove(pLocation);
	if (!explosionEnabled)
	{
		// 폭발타이머 시작거리
		if (distance < explosionStartDistance)
		{
			explosionEnabled = true;
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Explosion Start!"));
			light->ToggleVisibility();
		}
	}

	// 즉시폭발 최소거리
	if (distance < 100.0f)
	{
		Explosion();
	}
}

void AAIDog::ExplosionTimer(float t)
{
	if (explosionEnabled)
	{
		curExplosionTime -= t;
		if (curExplosionTime < 0.f)
		{
			Explosion();
		}
	}
}

void AAIDog::LightFlash(float t)
{
	if (explosionEnabled)
	{
		flashTime -= t;
		if (flashTime < 0)
		{
			light->ToggleVisibility(!light->IsVisible());
			// 깜박임 사운드
			UGameplayStatics::SpawnSoundAtLocation(this, flashSound, GetActorLocation());
			if (curExplosionTime * 0.2f > 0.1f)
			{
				flashTime = curExplosionTime * 0.2f;
			}
			else
			{
				flashTime = 0.1f;
			}
		}
	}
}

void AAIDog::AIMove(const FVector Destination)
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), Destination);
}

// Called when the game starts or when spawned
void AAIDog::BeginPlay()
{
	Super::BeginPlay();
	
	// 자기 라이트 넣기
	light = FindComponentByClass<UPointLightComponent>();
}

// Called every frame
void AAIDog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	StartExplosion();
	ExplosionTimer(DeltaTime);
	LightFlash(DeltaTime);
}

// Called to bind functionality to input
void AAIDog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAIDog::Explosion()
{
	//
	TArray<FHitResult> hitActors;

	// 충돌확인
	FCollisionShape SphereCol = FCollisionShape::MakeSphere(impactRadius);
	bool bSweepHit = GetWorld()->SweepMultiByChannel(hitActors, this->GetActorLocation(), this->GetActorLocation() + FVector(0.01f, 0.01f, 0.01f)
		, FQuat::Identity, ECC_WorldStatic, SphereCol);

	// 디버그 범위
	//DrawDebugSphere(GetWorld(), this->GetActorLocation(), impactRadius, 50, FColor::Red, false, 1.0f);

	// 충돌체들 확인용
	if (bSweepHit)
	{
		bool hitCheck = false;
		for (auto& Hit : hitActors)
		{
			UStaticMeshComponent* meshComp = Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent());
			if (meshComp)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, meshComp->GetName());
				meshComp->AddRadialImpulse(this->GetActorLocation(), impactRadius, impulseForce, ERadialImpulseFalloff::RIF_Constant, true);
			}

			if (!Hit.GetActor())
			{
				return;
			}


			if (Hit.GetActor()->ActorHasTag("Player") && !hitCheck)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Hit.GetActor()->GetName());

				auto temp = Hit.GetActor()->FindComponentByClass<UStatComponent>();
				if (temp) {
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("actor1 : %s"), *temp->GetName()));
					temp->Attacked(explosionDamage);
				}

				hitCheck = true;
			}
		}
	}

	// 폭발이펙트
	UGameplayStatics::SpawnEmitterAtLocation(this, explosionFX, GetActorLocation());
	// 폭발사운드
	UGameplayStatics::SpawnSoundAtLocation(this, explosionSound, GetActorLocation());

	Destroy();

}

