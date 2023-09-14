// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneAIController.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"

//#include "Navigation/PathFollowingComponent.h"
//#include "UObject/ObjectPtr.h"
//
//#include "NavFilters/NavigationQueryFilter.h"
//#include "GameFramework/Character.h"


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
	RunBehaviorTree(btree);
}

//EPathFollowingRequestResult::Type ADroneAIController::CustomMoveToLocation(const FVector& Dest, float AcceptanceRadius, bool bStopOnOverlap, bool bUsePathfinding, bool bProjectDestinationToNavigation, bool bCanStrafe, TSubclassOf<UNavigationQueryFilter> FilterClass, bool bAllowPartialPaths)
//{// abort active movement to keep only one request running
//	if (CustomPathFollowingComponent && CustomPathFollowingComponent->GetStatus() != EPathFollowingStatus::Idle)
//	{
//		CustomPathFollowingComponent->AbortMove(*this, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest
//			, FAIRequestID::CurrentRequest, EPathFollowingVelocityMode::Keep);
//	}
//
//	FAIMoveRequest MoveReq(Dest);
//	MoveReq.SetUsePathfinding(bUsePathfinding);
//	MoveReq.SetAllowPartialPath(bAllowPartialPaths);
//	MoveReq.SetProjectGoalLocation(bProjectDestinationToNavigation);
//	MoveReq.SetNavigationFilter(*FilterClass ? FilterClass : DefaultNavigationFilterClass);
//	MoveReq.SetAcceptanceRadius(AcceptanceRadius);
//	MoveReq.SetReachTestIncludesAgentRadius(bStopOnOverlap);
//	MoveReq.SetCanStrafe(bCanStrafe);
//
//
//	//Add
//	//ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
//	//FAIMoveRequest MoveReq(Dest); 
//	
//	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
//	MoveReq.UpdateGoalLocation(PlayerCharacter->GetActorLocation());
//
//
//	return MoveTo(MoveReq);
//}
//
