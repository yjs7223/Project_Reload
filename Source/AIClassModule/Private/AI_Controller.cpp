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
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Math/Vector.h"


AAI_Controller::AAI_Controller()
{
	PrimaryActorTick.bCanEverTick = true;

	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	HearingConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
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

	commander = nullptr;

	SetEnemy("Rifle_E");
}


void AAI_Controller::BeginPlay()
{
	Super::BeginPlay();
	/*APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	SetFocus(PlayerPawn);*/
	player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	UBlackboardComponent* BlackboardComp = Blackboard;
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		/*if (!RunBehaviorTree(btree))
			UE_LOG(LogTemp, Warning, TEXT("AIController couldn't run behavior tree!"));*/
	}

}
void AAI_Controller::OnTargetDetected(AActor* actor, FAIStimulus const Stimulus)
{
	switch (Stimulus.Type)
	{
	case 0:
		if (player)
		{
			DistanceToPlayer = GetPawn()->GetDistanceTo(player);
			UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceToPlayer);
			if (actor->ActorHasTag("Player"))
			{
				bIsPlayerDetected = Stimulus.WasSuccessfullySensed();
				if (Blackboard->GetValueAsObject("Target") != nullptr)
				{
					if (Cast<AActor>(Blackboard->GetValueAsObject("Target"))->ActorHasTag("Last"))
					{
						GetWorld()->DestroyActor(Cast<AActor>(Blackboard->GetValueAsObject("Target")));
					}
				}
				Blackboard->SetValueAsObject("Target", player);
			}
			if (actor->ActorHasTag("Last"))
			{
				if (commander->GetBlackboardComponent())
				{
					commander->GetBlackboardComponent()->SetValueAsObject("Cmd_Target", NULL);
					AActor* temp = Cast<AActor>(commander->GetBlackboardComponent()->GetValueAsObject("Cmd_Target"));
					GetWorld()->DestroyActor(temp);
				}
				bIsPlayerDetected = Stimulus.WasSuccessfullySensed();
			}

		}
		else {
			bIsPlayerDetected = false;
		}
		break;
	case 1:
		if (Stimulus.Tag == "Shooting")
		{
			Blackboard->SetValueAsObject("Target", player);
		}
		break;

	}
}

void AAI_Controller::SetUseCover()
{
	if (commander != nullptr)
	{
		if (commander->GetBlackboardComponent() != nullptr)
		{
			if (commander->CoverEnemyArray.Num() > 0)
			{
				for (auto loc : commander->CoverEnemyArray)
				{
					if (FVector::Distance(loc, GetPawn()->GetActorLocation()) <= 50)
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, GetPawn()->GetActorLocation().ToString());
						commander->GetBlackboardComponent()->SetValueAsBool("AI_UseCover", true);
					}
					else
					{
						commander->GetBlackboardComponent()->SetValueAsBool("AI_UseCover", false);
					}
				}
			}
		}
	}
}

void AAI_Controller::RunBTT()
{
	if (IsValid(btree)) 
	{
		RunBehaviorTree(btree);
	}
}

void AAI_Controller::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (DistanceToPlayer > SightConfig->LoseSightRadius)
	{
		Blackboard->SetValueAsObject("Target", nullptr);
		bIsPlayerDetected = false;
	}

	Blackboard->SetValueAsBool("Sight_In", bIsPlayerDetected);

	SetUseCover();
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
		HearingConfig->HearingRange = 10000.0f;
		HearingConfig->SetMaxAge(RangeData->Sight_Age);
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->ConfigureSense(*SightConfig);
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);	
		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAI_Controller::OnTargetDetected);
		
	}
}