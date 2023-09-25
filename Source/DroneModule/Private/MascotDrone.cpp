// Fill out your copyright notice in the Description page of Project Settings.


#include "MascotDrone.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "WeaponComponent.h"
//
//#include "Components/SceneComponent.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "DroneAIController.h"
#include "Engine/EngineTypes.h"

// Sets default values
AMascotDrone::AMascotDrone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_FloatingCmp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingCmp"));


	m_StaticMeshCmp.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body")));
	m_StaticMeshCmp.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Face")));
	m_StaticMeshCmp.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring")));
	m_StaticMeshCmp.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret")));
	m_StaticMeshCmp.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wing")));

	//GetMeshComponent
	m_StaticMeshCmp[0]->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/_sjs/Drone/DroneModel/Drone_Body.Drone_Body'")));
	m_StaticMeshCmp[1]->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/_sjs/Drone/DroneModel/Drone_Face.Drone_Face'")));
	m_StaticMeshCmp[2]->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/_sjs/Drone/DroneModel/Drone_Ring.Drone_Ring'")));
	m_StaticMeshCmp[3]->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/_sjs/Drone/DroneModel/Drone_Turret.Drone_Turret'")));
	m_StaticMeshCmp[4]->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/_sjs/Drone/DroneModel/Drone_Wing.Drone_Wing'")));


	m_StaticMeshCmp[0]->SetRelativeScale3D(FVector(.3f, .3f, .3f));

	for (int i = 1; i < m_StaticMeshCmp.Num(); i++)
	{
		m_StaticMeshCmp[i]->AttachToComponent(m_StaticMeshCmp[0], FAttachmentTransformRules::KeepRelativeTransform);
	}


	//sphere Collision
	CollisionMesh = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	CollisionMesh->InitSphereRadius(100.0f);
	CollisionMesh->AttachToComponent(m_StaticMeshCmp[0], FAttachmentTransformRules::KeepRelativeTransform);


	//나이아가라 
	m_Empcmp = CreateDefaultSubobject<USceneComponent>(TEXT("Empcmp"));


	m_EmpEffect.Add(CreateDefaultSubobject<UNiagaraComponent>(TEXT("PreExplosion")));
	m_EmpEffect.Add(CreateDefaultSubobject<UNiagaraComponent>(TEXT("Explosion")));
	

	m_EmpEffect[0]->SetAsset(LoadObject<UNiagaraSystem>(NULL, TEXT("NiagaraSystem'/Game/_sjs/Drone/NS_Electrical_PreExplosion.NS_Electrical_PreExplosion'")));
	m_EmpEffect[1]->SetAsset(LoadObject<UNiagaraSystem>(NULL, TEXT("NiagaraSystem'/Game/_sjs/Drone/NS_Electrical_Explosion.NS_Electrical_Explosion'")));
	

	for (auto& i : m_EmpEffect)
	{
		i->AttachToComponent(m_Empcmp, FAttachmentTransformRules::KeepRelativeTransform);
	}

	m_Empcmp->AttachToComponent(m_StaticMeshCmp[0], FAttachmentTransformRules::KeepRelativeTransform);


	//AI컨트롤러 세팅
	AIControllerClass = ADroneAIController::StaticClass();

	////왜안먹히누
	m_EmpEffect[0]->Deactivate();
	m_EmpEffect[1]->Deactivate();

}

// Called when the game starts or when spawned
void AMascotDrone::BeginPlay()
{
	Super::BeginPlay();


	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AMascotDrone::OverlapBegin);
	CollisionMesh->OnComponentEndOverlap.AddDynamic(this, &AMascotDrone::OverlapEnd);

	m_EmpEffect[0]->Activate();
	m_EmpEffect[1]->Deactivate();

	//BT실행
	Cast<ADroneAIController>(GetController())->RunBTT();
	
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
	m_EmpEffect[0]->Deactivate();
	if (m_EmpEffect[1]->IsActive())
	{
		m_EmpEffect[1]->Deactivate();
	}

	if (EmpDelay == false)
	{
		m_EmpEffect[1]->Activate();
		EmpDelay = true;


		FTimerHandle EMPTimerHandle;
		float EMPDelayTime = 3;
		if (1)
		{
			GetWorld()->GetTimerManager().SetTimer(EMPTimerHandle, FTimerDelegate::CreateLambda([&]()
				{
					// 코드 구현
					m_EmpEffect[0]->Activate();
					m_EmpEffect[1]->Deactivate();


					EmpDelay = false;
					// TimerHandle 초기화
					GetWorld()->GetTimerManager().ClearTimer(EMPTimerHandle);
				}), EMPDelayTime, false);
		}
	}

	return m_NearAI;
}
