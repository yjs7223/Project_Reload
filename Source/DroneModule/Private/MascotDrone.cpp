// Fill out your copyright notice in the Description page of Project Settings.


#include "MascotDrone.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AMascotDrone::AMascotDrone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_FloatingCmp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingCmp"));
	m_StaticMeshCmp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshCmp"));



	////Mesh
	//static ConstructorHelpers::FObjectFinder<UStaticMesh>
	//	m_Mesh(TEXT("StaticMesh'/Game/_sjs/Drone/Drone_05/Mesh/SM_Drone_05.SM_Drone_05'"));

	//m_StaticMeshCmp->SetStaticMesh(m_Mesh.Object);

}

// Called when the game starts or when spawned
void AMascotDrone::BeginPlay()
{
	Super::BeginPlay();

	
	
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

