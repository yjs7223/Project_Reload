// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAICtr.h"
#include "ST_Range.h"
#include "BaseCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


const FName ABaseAICtr::Key_Cover(TEXT("Key_Cover"));
const FName ABaseAICtr::Key_CoverLocation(TEXT("Key_CoverLocation"));

ABaseAICtr::ABaseAICtr()
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

	/*static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/_sjs/BT_BaseAI.BT_BaseAI'"));
	if (BTObject.Succeeded())
		BTAsset = BTObject.Object;*/

}



void ABaseAICtr::BeginPlay()
{

	Super::BeginPlay();
	if (GetPerceptionComponent() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALL Systems Set!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Component"));
	}
}

void ABaseAICtr::OnPossess(APawn* pPawn)
{
	Super::OnPossess(pPawn);
	RunBehaviorTree(btree);
	behavior_tree_component->StartTree(*btree);
	UBlackboardComponent* BlackboardComp = Blackboard;
	if (UseBlackboard(BBAsset, BlackboardComponent))
	{
		if (!RunBehaviorTree(BTAsset))
			UE_LOG(LogTemp, Warning, TEXT("AIController couldn't run behavior tree!"));
	}
}

void ABaseAICtr::Tick(float DeltaSeconds)
{
	m_character = Cast<ABaseCharacter>(GetPawn());
	BlackboardComponent = Blackboard;
	if (DistanceToPlayer > SightConfig->LoseSightRadius)
	{
		bIsPlayerDetected = false;
	}
	if (bIsPlayerDetected)
	{
		m_character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		BlackboardComponent->SetValueAsObject("Target", m_character);
	}
	
	BlackboardComponent->SetValueAsBool("Sight_In", bIsPlayerDetected);
	
}

FRotator ABaseAICtr::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.f, 0.f, 0.f);
	}

	return FRotator(0.f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void ABaseAICtr::SetEnemy(FName EnemyName)
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
		GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ABaseAICtr::OnPawnDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
		/*SightConfig->SightRadius = RangeData->Sense_Radius;
		SightConfig->SightRadius = 360.0f - RangeData->Sight_Angle;
		SightConfig->SightRadius = RangeData->Sight_Age;*/
	}
}

void ABaseAICtr::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
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
