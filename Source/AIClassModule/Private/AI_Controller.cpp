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
//#include "Perception/AIPerceptionSystem.h"
//#include "Perception/AISenseConfig_Sight.h"
//#include "Perception/AISenseConfig_Hearing.h"
//#include "Perception/AIPerceptionStimuliSourceComponent.h"
//#include "Perception/AIPerceptionComponent.h"
#include "Math/Vector.h"
#include "SubEncounterSpace.h"
#include "AISpawner.h"
#include "AICharacterMoveComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"
#include "Sound/SoundCue.h"
#include "Navigation/CrowdFollowingComponent.h"


AAI_Controller::AAI_Controller(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	PrimaryActorTick.bCanEverTick = true;

	/*SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	HearingConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));*/

	//static ConstructorHelpers::FObjectFinder<UDataTable> DT_RangeDataObject(TEXT("DataTable'/Game/AI_Project/DT/DT_Range.DT_Range'"));
	//if (DT_RangeDataObject.Succeeded())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
	//	DT_Range = DT_RangeDataObject.Object;
	//}

	//BT
	/*static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/AI_Project/AI_Pakage/BaseAI/BT/BT_Main.BT_Main'"));
	if (BTObject.Succeeded())
	{
		btree = BTObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("BehaviorTree Succeed!"));
	}
	behavior_tree_component = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	*/
	BBAsset = LoadObject<UBlackboardData>(NULL, TEXT("BlackboardData'/Game/AI_Project/AI_Pakage/BaseAI/BB/BB_BaseAI.BB_BaseAI'"));
	

	commander = nullptr;
	em_normal = false;
	//SetEnemy("Rifle_E");
}

void AAI_Controller::BeginPlay()
{
	Super::BeginPlay();
	/*APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	SetFocus(PlayerPawn);*/
	player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//DistanceToPlayer = 0.0f;
	UBlackboardComponent* BlackboardComp = Blackboard;
	UseBlackboard(BBAsset, BlackboardComp);
	playerMesh = player->FindComponentByClass<USkeletalMeshComponent>();
	Blackboard->SetValueAsVector("AI_MoveLocation", FVector::ZeroVector);
	Blackboard->SetValueAsVector("AI_CoverLocation", FVector::ZeroVector);

	GetWorldTimerManager().ClearTimer(timer);
	GetWorldTimerManager().SetTimer(timer, this, &AAI_Controller::SetUseCover, 0.6, true, 0.0f);
}

//void AAI_Controller::OnTargetDetected(AActor* actor, FAIStimulus Stimulus)
//{
//	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, FString::FromInt(Stimulus.Type.Index));
//	if (commander == nullptr)
//	{
//		return;
//	}
//	if (commander->m_suben == nullptr)
//	{
//		return;
//	}
//	switch (Stimulus.Type)
//	{
//	case 0:
//		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, "SIGHTSIGHT");
//		if (player == nullptr)
//		{
//			bIsPlayerDetected = false;
//			break;
//		}
//		if ((commander->m_suben->GetActorLocation().X - commander->m_suben->CollisionMesh->GetScaledBoxExtent().X) <= player->GetActorLocation().X
//			&& (commander->m_suben->GetActorLocation().X + commander->m_suben->CollisionMesh->GetScaledBoxExtent().X) >= player->GetActorLocation().X)
//		{
//			if ((commander->m_suben->GetActorLocation().Y - commander->m_suben->CollisionMesh->GetScaledBoxExtent().Y) <= player->GetActorLocation().Y
//				&& (commander->m_suben->GetActorLocation().Y + commander->m_suben->CollisionMesh->GetScaledBoxExtent().Y) >= player->GetActorLocation().Y)
//			{
//				DistanceToPlayer = GetPawn()->GetDistanceTo(player);
//				UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceToPlayer);
//				if (actor->ActorHasTag("Player"))
//				{
//					bIsPlayerDetected = Stimulus.WasSuccessfullySensed();
//					if (Blackboard->GetValueAsObject("Target") != nullptr)
//					{
//						if (Cast<AActor>(Blackboard->GetValueAsObject("Target"))->ActorHasTag("Last"))
//						{
//							GetWorld()->DestroyActor(Cast<AActor>(Blackboard->GetValueAsObject("Target")));
//						}
//					}
//					Blackboard->SetValueAsObject("Target", player);
//				}
//				else {
//					bIsPlayerDetected = false;
//				}
//			}
//			else {
//				bIsPlayerDetected = false;
//			}
//		}
//		else {
//			bIsPlayerDetected = false;
//		}
//		break;
//		// react to sight stimulus
//	case 1:
//
//		if ((commander->m_suben->GetActorLocation().X - commander->m_suben->CollisionMesh->GetScaledBoxExtent().X) <= player->GetActorLocation().X
//			&& (commander->m_suben->GetActorLocation().X + commander->m_suben->CollisionMesh->GetScaledBoxExtent().X) >= player->GetActorLocation().X)
//		{
//			if ((commander->m_suben->GetActorLocation().Y - commander->m_suben->CollisionMesh->GetScaledBoxExtent().Y) <= player->GetActorLocation().Y
//				&& (commander->m_suben->GetActorLocation().Y + commander->m_suben->CollisionMesh->GetScaledBoxExtent().Y) >= player->GetActorLocation().Y)
//			{
//				if (Blackboard->GetValueAsBool("AI_Active"))
//				{
//					if (!Stimulus.Tag.IsValid())
//					{
//						break;
//					}
//					if (Stimulus.Tag == "Shooting")
//					{
//						Blackboard->SetValueAsObject("Target", player);
//					}
//				}
//			}
//		}
//		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, "HearingHearing");
//		break;
//		// react to hearing;
//	default:
//		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, "HearingHearing");
//		return;
//	}
//	
//	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, Stimulus.Tag.ToString());
//	
//}

void AAI_Controller::SetUseCover()
{
	if (GetBlackboardComponent()->GetValueAsBool("AI_Active"))
	{
		FCollisionQueryParams collisionParams;

		FVector correction;
	
		correction = FVector(0, 0, 20);
		

		FVector start = GetPawn()->GetActorLocation() - correction;

		collisionParams.AddIgnoredActor(GetPawn());

		//FVector playerLocation = playerMesh->GetSocketLocation(TEXT("head"));
		FVector playerLocation = player->GetActorLocation();
			

		float size = 3;

		if (GetPawn()->GetDistanceTo(player) <= 500)
		{
			GetBlackboardComponent()->SetValueAsBool("AI_UseCover", false);
			/*DrawDebugCapsule(GetWorld(), GetPawn()->GetActorLocation(), GetPawn()->GetDistanceTo(player)
				, size, FRotationMatrix::MakeFromZ(GetPawn()->GetActorLocation() - playerLocation).ToQuat(), FColor::Red, false, 0.6f);*/
		}
		else if (GetWorld()->SweepSingleByChannel(result, start, playerLocation, GetPawn()->GetActorRotation().Quaternion(), ECC_Visibility,
			FCollisionShape::MakeSphere(size), collisionParams))
		{
			if (result.GetActor()->ActorHasTag("Player"))
			{
				//DrawDebugLine(GetWorld(), start, playerLocation, FColor::Blue, false, 0.1f);

				GetBlackboardComponent()->SetValueAsBool("AI_UseCover", false);

				/*DrawDebugCapsule(GetWorld(), GetPawn()->GetActorLocation(), GetPawn()->GetDistanceTo(player)
					, size, FRotationMatrix::MakeFromZ(GetPawn()->GetActorLocation() - playerLocation).ToQuat(), FColor::Red, false, 0.6f);*/
			}
			else
			{
				if (FVector::Distance(GetPawn()->GetActorLocation(), result.ImpactPoint) < 250.0f)
				{
					GetBlackboardComponent()->SetValueAsBool("AI_UseCover", true);
					//DrawDebugLine(GetWorld(), start, playerLocation, FColor::Red, false, 0.1f);

					/*DrawDebugCapsule(GetWorld(), GetPawn()->GetActorLocation(), GetPawn()->GetDistanceTo(player)
						, size, FRotationMatrix::MakeFromZ(GetPawn()->GetActorLocation() - playerLocation).ToQuat(), FColor::Cyan, false, 0.6f);*/
				}
				else
				{
					GetBlackboardComponent()->SetValueAsBool("AI_UseCover", false);
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow , TEXT("false"));
					//DrawDebugLine(GetWorld(), start, playerLocation, FColor::Blue, false, 0.1f);

					/*DrawDebugCapsule(GetWorld(), GetPawn()->GetActorLocation(), GetPawn()->GetDistanceTo(player)
						, size, FRotationMatrix::MakeFromZ(GetPawn()->GetActorLocation() - playerLocation).ToQuat(), FColor::Red, false, 0.6f);*/
				}
			}
		}
	}
}

void AAI_Controller::RunBTT()
{
	RunBehaviorTree(btree);
}


void AAI_Controller::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, GetControlRotation().Vector().ToString());
	/*if (DistanceToPlayer > SightConfig->LoseSightRadius)
	{
		Blackboard->SetValueAsObject("Target", nullptr);
		bIsPlayerDetected = false;
	}*/
	if (Blackboard->GetValueAsObject("Target") != nullptr)
	{
		//DistanceToPlayer = 0.0f;
		if (GetPawn()->FindComponentByClass<UAICharacterMoveComponent>())
		{
			if (em_normal == false)
			{
				if (GetPawn()->FindComponentByClass<UAICharacterMoveComponent>()->e_move != EMove::Hit)
				{
					GetPawn()->FindComponentByClass<UAICharacterMoveComponent>()->e_move = EMove::Normal;
				}
				em_normal = true;
			}
			
		}
		
	}
	//Blackboard->SetValueAsBool("Sight_In", bIsPlayerDetected);
}

//void AAI_Controller::PlayVoiceSound(Voice sound, int random)
//{
//	float pitch = FMath::RandRange(0, random);
//	if (pitch == 0)
//	{
//		UGameplayStatics::PlaySoundAtLocation(this, FireSound[(int)sound], GetPawn()->GetActorLocation(), 0.5f, pitch);
//	}
//}

//FRotator AAI_Controller::GetControlRotation() const
//{
//	if (GetPawn() == nullptr)
//	{
//		return FRotator(0.f, 0.f, 0.f);
//	}
//
//	return FRotator(0.f, GetPawn()->GetActorRotation().Yaw, 0.0f);
//}

//void AAI_Controller::SetEnemy(FName EnemyName)
//{
//	FST_Range* RangeData = DT_Range->FindRow<FST_Range>(EnemyName, FString(""));
//	if (RangeData)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("EnemyData Succeed!"));
//
//		SightConfig->SightRadius = RangeData->Sight_Radius;
//		SightConfig->LoseSightRadius = RangeData->LoseSight_Radius;
//		SightConfig->PeripheralVisionAngleDegrees = RangeData->Sight_Angle;
//		SightConfig->SetMaxAge(RangeData->Sight_Age);
//		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
//		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
//		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
//		HearingConfig->HearingRange = 3000.0f;
//		HearingConfig->SetMaxAge(RangeData->Sight_Age);
//		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
//		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
//		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
//
//		/*GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAI_Controller::OnTargetDetected);
//		GetPerceptionComponent()->ConfigureSense(*SightConfig);
//		GetPerceptionComponent()->ConfigureSense(*HearingConfig);	
//		GetPerceptionComponent()->SetDominantSense(UAISenseConfig_Sight::StaticClass());*/
//		
//		
//	}
//}