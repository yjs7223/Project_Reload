// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneAIController.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"



ADroneAIController::ADroneAIController()
{
	//bt Setting
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
		BTObject(TEXT("BehaviorTree'/Game/_sjs/Drone/BT_DroneAI.BT_DroneAI'"));

	if (BTObject.Succeeded())
	{
		btree = BTObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("BehaviorTree Succeed!"));
	}




}

void ADroneAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBTT();
}

void ADroneAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ADroneAIController::RunBTT()
{
	//RunBehaviorTree(btree);
}

bool ADroneAIController::DroneMoveTo(FVector p_vec, float p_radius)
{
	bool isArrive = false;

	MoveToLocation(p_vec,p_radius,false,false);

	/*DrawDebugBox(GetWorld(), p_vec, FVector(10, 10, 10), FColor::Red);
	DrawDebugBox(GetWorld(), GetPawn()->GetActorLocation(), FVector(10, 10, 10), FColor::Blue);*/

	float test = FVector::Dist(GetPawn()->GetActorLocation(), p_vec);
	if (p_radius > FVector::Dist(GetPawn()->GetActorLocation(), p_vec))
	{
		isArrive = true;
	}


	return isArrive;
}

bool ADroneAIController::DroneFocus(float time)
{

	AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn();

	//FMath::RInterpTo(GetActorRotation,)
	return false;
}
