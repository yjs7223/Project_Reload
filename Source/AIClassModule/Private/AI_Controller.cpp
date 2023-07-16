// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Controller.h"
#include "ST_Range.h"
#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"


AAI_Controller::AAI_Controller()
{
	PrimaryActorTick.bCanEverTick = true;

	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_RangeDataObject(TEXT("DataTable'/Game/Aws/AI_Stat/DT_Range.DT_Range'"));
	if (DT_RangeDataObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
		DT_Range = DT_RangeDataObject.Object;
	}

	//BT
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/JHB/BTBt.BTBt'"));
	if (BTObject.Succeeded())
	{
		btree = BTObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("BehaviorTree Succeed!"));
	}
	behavior_tree_component = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB_BaseAIObject(TEXT("BlackBoard'/Game/JHB/BB_BaseAI.BB_BaseAI'"));
	if (BB_BaseAIObject.Succeeded())
	{
		BBAsset = BB_BaseAIObject.Object;
	}



	SetEnemy("Rifle_E");
}

void AAI_Controller::BeginPlay()
{
	Super::BeginPlay();
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	SetFocus(PlayerPawn); // 플레이어 폰의 위치로 SetFocus
	m_character = Cast<ABaseCharacter>(GetPawn());
	RunBehaviorTree(btree);
	behavior_tree_component->StartTree(*btree);
	UBlackboardComponent* BlackboardComp = Blackboard;
	if (UseBlackboard(BBAsset, BlackboardComponent))
	{
		if (!RunBehaviorTree(btree))
			UE_LOG(LogTemp, Warning, TEXT("AIController couldn't run behavior tree!"));
	}
		
}

void AAI_Controller::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	BlackboardComponent = Blackboard;


	if (DistanceToPlayer > SightConfig->LoseSightRadius)
	{
		bIsPlayerDetected = false;
	}
	if (bIsPlayerDetected)
	{
		BlackboardComponent->SetValueAsObject("Target", m_character);
	}

	BlackboardComponent->SetValueAsBool("Sight_In", bIsPlayerDetected);
}

void AAI_Controller::SetEnemy(FName EnemyName)
{
	FST_Range* RangeData = DT_Range->FindRow<FST_Range>(EnemyName, FString(""));
	if (RangeData)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData Succeed!"));

		SightConfig->SightRadius = RangeData->Sight_Radius;
		SightConfig->LoseSightRadius = RangeData->LoseSight_Radius;
		SightConfig->PeripheralVisionAngleDegrees = RangeData->Sight_Angle;
		SightConfig->SetMaxAge(RangeData->Sight_Age);
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AAI_Controller::OnPawnDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
		/*SightConfig->SightRadius = RangeData->Sense_Radius;
		SightConfig->SightRadius = 360.0f - RangeData->Sight_Angle;
		SightConfig->SightRadius = RangeData->Sight_Age;*/
	}
}

void AAI_Controller::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	for (size_t i = 0; i < DetectedPawns.Num(); i++)
	{
		if (DetectedPawns[i]->ActorHasTag("Player"))
		{
			DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);
			UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceToPlayer);
			bIsPlayerDetected = true;
		}
	}
}
