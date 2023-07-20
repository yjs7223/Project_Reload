// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Controller.h"
#include "AICharacter.h"
#include "AICommander.h"
#include "ST_Range.h"
#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
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
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/SGJ/BT_Main.BT_Main'"));
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
	/*APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	SetFocus(PlayerPawn);*/
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
void AAI_Controller::OnTargetDetected(AActor* actor, FAIStimulus const Stimulus)
{
	if (auto const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		DistanceToPlayer = GetPawn()->GetDistanceTo(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceToPlayer);
		if (actor->ActorHasTag("Player"))
		{
			bIsPlayerDetected = Stimulus.WasSuccessfullySensed();
		}
		if (actor->ActorHasTag("Last"))
		{
			AIController = nullptr;
			ACharacter = Cast<AAICharacter>(Cast<AAICommander>(commander));
			if (ACharacter)
			{
				AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ACharacter)->GetController());
			}
			if (AIController)
			{
				if (AIController->BlackboardComponent)
				{
					BlackboardComponent = AIController->BlackboardComponent;
					BlackboardComponent->SetValueAsObject("Cmd_Target", NULL);
					AActor* temp = Cast<AActor>(BlackboardComponent->GetValueAsObject("Cmd_Target"));
					GetWorld()->DestroyActor(temp);
				}
			}

			bIsPlayerDetected = Stimulus.WasSuccessfullySensed();
		}

	}
	else {
		bIsPlayerDetected = false;
	}

	/*for (size_t i = 0; i < DetectedPawns.Num(); i++)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, DetectedPawns[i]->GetName());
		if (DetectedPawns[i]->ActorHasTag("Player"))
		{
			DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);
			UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceToPlayer);

			bIsPlayerDetected = true;
		}
	}
	if(SightConfig->age)*/

}

void AAI_Controller::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	m_character = Cast<ABaseCharacter>(GetPawn());
	BlackboardComponent = Blackboard;


	if (DistanceToPlayer > SightConfig->LoseSightRadius)
	{
		BlackboardComponent->SetValueAsObject("Target", nullptr);
		bIsPlayerDetected = false;
	}
	if (bIsPlayerDetected)
	{
		m_character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		BlackboardComponent->SetValueAsObject("Target", m_character);
	}

	BlackboardComponent->SetValueAsBool("Sight_In", bIsPlayerDetected);
}

FRotator AAI_Controller::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.f, 0.f, 0.f);
	}

	return FRotator(0.f, GetPawn()->GetActorRotation().Yaw, 0.0f);
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
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAI_Controller::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
		/*SightConfig->SightRadius = RangeData->Sense_Radius;
		SightConfig->SightRadius = 360.0f - RangeData->Sight_Angle;
		SightConfig->SightRadius = RangeData->Sight_Age;*/
	}
}