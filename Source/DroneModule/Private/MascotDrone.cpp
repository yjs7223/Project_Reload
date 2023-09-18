// Fill out your copyright notice in the Description page of Project Settings.


#include "MascotDrone.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "WeaponComponent.h"

#include "Components/SphereComponent.h"
#include "DroneAIController.h"


// Sets default values
AMascotDrone::AMascotDrone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_FloatingCmp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingCmp"));
	m_StaticMeshCmp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshCmp"));

	





	//GetMeshComponent
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		MeshAsset(TEXT("StaticMesh'/Game/_sjs/Drone/Drone_05/Mesh/SM_Drone_05.SM_Drone_05'"));
	m_StaticMeshCmp->SetStaticMesh(MeshAsset.Object);


	//sphere Collision
	CollisionMesh = CreateDefaultSubobject<USphereComponent>(FName("Sphere"));
	CollisionMesh->InitSphereRadius(100.0f);

	CollisionMesh->AttachToComponent(m_StaticMeshCmp,FAttachmentTransformRules::KeepRelativeTransform);


}

// Called when the game starts or when spawned
void AMascotDrone::BeginPlay()
{
	Super::BeginPlay();


	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AMascotDrone::OverlapBegin);
		CollisionMesh->OnComponentEndOverlap.AddDynamic(this, &AMascotDrone::OverlapEnd);
	
}

// Called every frame
void AMascotDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

// Called to bind functionality to input
void AMascotDrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMascotDrone::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UWeaponComponent::CheckActorTag(OtherActor, TEXT("Enemy")))
	{
		if (!UWeaponComponent::CheckActorTag(OtherActor, TEXT("Barrel")))
			if (!UWeaponComponent::CheckActorTag(OtherActor, TEXT("Spider")))
				m_NearAI.Add(OtherActor);
	}
}

void AMascotDrone::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (UWeaponComponent::CheckActorTag(OtherActor, TEXT("Enemy")))
	{
		if (!UWeaponComponent::CheckActorTag(OtherActor, TEXT("Barrel")))
			if (!UWeaponComponent::CheckActorTag(OtherActor, TEXT("Spider")))
				if(m_NearAI.Find(OtherActor))
					m_NearAI.Remove(OtherActor);
	}
}

TArray<AActor*> AMascotDrone::EMP()
{
	return m_NearAI;
}
