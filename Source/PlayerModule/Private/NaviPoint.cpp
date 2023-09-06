// Fill out your copyright notice in the Description page of Project Settings.


#include "NaviPoint.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
ANaviPoint::ANaviPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	spherecomp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
}

// Called when the game starts or when spawned
void ANaviPoint::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ANaviPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	NearDestroy();
}

void ANaviPoint::NearDestroy()
{
	ACharacter* pc = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (pc)
	{
		FVector dis = pc->GetActorLocation() - GetActorLocation();
		float length = dis.Length();
		if (length < 200)
		{
			Destroy();
		}
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(length));
	}
}


