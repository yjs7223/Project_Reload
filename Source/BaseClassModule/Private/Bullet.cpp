// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "WeaponComponent.h"


// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(65.0f);
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Bullet"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 30000.0f;
	ProjectileMovementComponent->MaxSpeed = 30000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true; 
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	InitialLifeSpan = 3.0f;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	//owner = Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	ProjectileMovementComponent;
	prev_loc = GetActorLocation();
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	InitialLifeSpan = 3.0f;

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnSphereBeginOverlap);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HitCheck();
}

void ABullet::SpawnBulletFx(UNiagaraSystem* BulletFXNiagara, const FVector& ShootDirection, class ABaseCharacter* p_owner, USoundCue* passby)
{
	if (BulletFXNiagara)
	{
		owner = p_owner;
		BulletFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(BulletFXNiagara, CollisionComponent, FName("none"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
		passbySound = passby;
	}
}

void ABullet::SpawnBulletFx(UNiagaraSystem* BulletFXNiagara, const FVector& ShootDirection, ABaseCharacter* p_owner)
{
	if (BulletFXNiagara)
	{
		owner = p_owner;
		BulletFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(BulletFXNiagara, CollisionComponent, FName("none"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
		
	}
}

void ABullet::HitCheck()
{
	FVector now_loc = GetActorLocation();
	FHitResult result;
	FCollisionQueryParams param(NAME_None, true, this);
	if (GetWorld()->LineTraceSingleByChannel(result, prev_loc, now_loc, ECC_GameTraceChannel6, param))
	{
		if (result.GetActor() != owner)
		{
			
			if (UWeaponComponent::CheckActorTag(result.GetActor(), TEXT("Vehicle")))
			{
				if (result.GetComponent()->ComponentTags.Num() > 0)
				{
					if (result.GetComponent()->ComponentHasTag(TEXT("Plate")))
					{
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, TEXT("Plate"));
						int rand = FMath::RandRange(0, 100);
						if (rand <= 75)
						{
							GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, TEXT("15"));
							Cast<UStaticMeshComponent>(result.GetComponent())->SetSimulatePhysics(true);
						}
					}
				}
			}

			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, result.GetActor()->GetName());
			OnBulletHitDelegate.Broadcast(result);
			Destroy();

		}
		
	}
	prev_loc = now_loc;

}

void ABullet::PlayPassbySound(FVector p_loc)
{
	if (passbySound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, passbySound, p_loc);
	}
}

void ABullet::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && UWeaponComponent::CheckActorTag(OtherActor, "Player"))
	{
		FVector start = GetActorLocation();
		FVector end = start + start.ForwardVector * 100.0f;
		FHitResult result;
		FCollisionQueryParams param(NAME_None, true, this);
		if (GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_GameTraceChannel6, param))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("hitpassby"));
			return;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("passby"));
		PlayPassbySound(SweepResult.Location);
	}
}


