// Fill out your copyright notice in the Description page of Project Settings.


#include "AIZombie.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "AIStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "AISpawner.h"

// Sets default values
AAIZombie::AAIZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAIZombie::BeginPlay()
{
	Super::BeginPlay();
	
	player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    collisionMesh = FindComponentByClass<UCapsuleComponent>();
}

// Called every frame
void AAIZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (attack)
    {
        if (GetController())
        {
            UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), GetActorLocation());
        }
    }
    else if (!FindComponentByClass<UAIStatComponent>()->isDie)
    {
        if (target != nullptr)
        {
            UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), target->GetActorLocation());
        }
    }
    else
    {
        UPrimitiveComponent* CollisionComponent = Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass()));

        if (CollisionComponent)
        {
            CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
        }
    }

    if (GetDistanceTo(player) <= 150)
    {
        attack = true;
    }
    else if (GetDistanceTo(player) >= 500)
    {
        attack = false;
    }

    if (mySpawner != nullptr)
    {
        if (mySpawner->many_Spawn)
        {
            target = mySpawner->spawn_Spots[1];
        }
    }
}

// Called to bind functionality to input
void AAIZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAIZombie::BulletHit(FHitResult result)
{
    // 블루프린트 클래스의 경로 및 이름
    FString BlueprintPath = TEXT("Blueprint'/Game/AI_Project/AI_Pakage/BaseAI/BP/BP_AIZombie.BP_AIZombie'");

    // 블루프린트 클래스 로딩
    UBlueprint* BlueprintAsset = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), NULL, *BlueprintPath));

    if (BlueprintAsset)
    {
        // 블루프린트 클래스의 CDO(클래스 디폴트 오브젝트) 생성
        UObject* BlueprintObj = BlueprintAsset->GeneratedClass->GetDefaultObject();

        // 커스텀 이벤트 호출
        if (BlueprintObj)
        {
            FVector impulse = (player->GetActorLocation() - GetActorLocation()) * 500.0f;
            BlueprintObj->ProcessEvent(BlueprintObj->FindFunction(FName("Bullet Hit Event")), &result.BoneName);
        }
    }
}

