// Fill out your copyright notice in the Description page of Project Settings.

#include "AICommander.h"
#include "AICharacter.h"
#include "AIStatComponent.h"
#include "EncounterSpace.h"
#include "SubEncounterSpace.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "ST_Suppression.h"
#include "ST_Commander.h"
#include "Components/CapsuleComponent.h"
#include "AICharacterMoveComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "EngineGlobals.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI_Controller.h"
#include "CoverComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Vector.h"
#include "Components/PrimitiveComponent.h"
#include "AIPatrolComponent.h"
#include "AISensingComponent.h"
#include "AIWeaponComponent.h"
#include "AIInputComponent.h"
#include "CoverComponent.h"
#include "CoverManager.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"


// Sets default values
AAICommander::AAICommander()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//encounter = CreateDefaultSubobject<AEncounterSpace>(TEXT("Encounter"));
	PrimaryActorTick.bCanEverTick = true;

	AddIndex = 0;
	MapList_Start = false;
	sightin = false;
	enemycover = false;
	SightIn_CHK = false;
	Cmd_SightOut = false;
	//Patrol_CHK = false;
	static ConstructorHelpers::FObjectFinder<UBlueprint> RedBallData(TEXT("Blueprint'/Game/JHB/RedBall.RedBall'"));
	if (RedBallData.Succeeded())
	{
		RedBallBlueprint = (UClass*)RedBallData.Object->GeneratedClass;
	}


	DT_Commander = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/AI_Project/DT/DT_Commander.DT_Commander'"));
	//BT
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/AI_Project/AI_Pakage/Commander/BT/BT_AICommander.BT_AICommander'"));

	btree = LoadObject<UBehaviorTree>(NULL, TEXT("BehaviorTree'/Game/AI_Project/AI_Pakage/Commander/BT/BT_AICommander.BT_AICommander'"));
	
	behavior_tree_component = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

	BB_AICommander = LoadObject<UBlackboardData>(NULL, TEXT("BlackboardData'/Game/AI_Project/AI_Pakage/Commander/BB/BB_Commander.BB_Commander'"));
	
	
	audiocomp = CreateDefaultSubobject<UAudioComponent>(TEXT("audiocomp"));
	
}

// Called when the game starts or when spawned
void AAICommander::BeginPlay()
{
	Super::BeginPlay();
	player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEncounterSpace::StaticClass(), EncounterArray);
	
	CoverManager = Cast<ACoverManager>
		(UGameplayStatics::GetActorOfClass(GetWorld(), ACoverManager::StaticClass()));

	
	UBlackboardComponent* BlackboardComp = Blackboard;
	UseBlackboard(BB_AICommander, BlackboardComp);
	RunBehaviorTree(btree);
	behavior_tree_component->StartTree(*btree);
	audiocomp->Stop();
	background = LoadObject<USoundCue>(NULL, TEXT("SoundCue'/Game/AI_Project/AI_Pakage/BaseAI/Sound/NonCombatting_Cue.NonCombatting_Cue'"));
	audiocomp->SetSound(background);
	audiocomp->Play();
	audiocomp->FadeIn(2.0f);
	sound_Start = false;
	SetCommanderDataTable("Commander");
}


void AAICommander::SetCommanderDataTable(const FName EnemyName)
{
	FST_Commander* CommanderData = DT_Commander->FindRow<FST_Commander>(EnemyName, FString(""));
	if (CommanderData)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData Succeed!"));

		siege_range = CommanderData->Siege_Range;
		detour_range = CommanderData->Detour_Range;
		detour_angle = CommanderData->Detour_Angle;
		ndetour_angle = CommanderData->Ndetour_Angle;
	}
}

// Called every frame
void AAICommander::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	s_time += DeltaTime;
	
	ListSet();
}

void AAICommander::ListSet()
{
	if (m_suben != nullptr)
	{
		if (Now_suben != m_suben)
		{
			Now_suben = m_suben;
			Now_suben->EncounterAIActive = true;
			m_suben = nullptr;
		}
		else m_suben = nullptr;

	}

	if (m_en != nullptr)
	{
		if (Now_en != m_en)
		{
			ListVoidReset();
			Now_en = m_en;
			MapList_Start = false;
			CoverManager->ChangeEncounter();
			audiocomp->Stop();
			/*if (Now_en->AIArray.Num() <= 0)
			{
				USoundCue* background = LoadObject<USoundCue>(NULL, TEXT("SoundCue'/Game/AI_Project/AI_Pakage/BaseAI/Sound/NonCombatting_Cue.NonCombatting_Cue'"));
				audiocomp->SetSound(background);
			}*/
			//else
			//{
				background = LoadObject<USoundCue>(NULL, TEXT("SoundCue'/Game/yjs/Sounds/S_Fire_Support_LOOP_150bpm_Cue.S_Fire_Support_LOOP_150bpm_Cue'"));
				audiocomp->SetSound(background);
			//}
			audiocomp->Play();
			audiocomp->FadeIn(2.0f);
			sound_Start = true;
			m_en = nullptr;
		}
		else
		{
			m_en = nullptr;
		}
	}
	
	if (Now_en == nullptr)
	{
		return;
	}
	if (Now_suben == nullptr)
	{
		return;
	}
	
	Blackboard->SetValueAsObject("Cmd_Space", Now_en);

	if (Now_en->spawn)
	{
		Now_en->spawn->check_Overlap = true;
	}
	if (!MapList_Start)
	{
		ListStartSet(Now_en);
	}
	else
	{
		ListTickSet();
		TargetTickSet();
		CoverPointEnemy();
		if (List_Division.Num() <= 0)
		{
			if (Now_en->spawn != nullptr)
			{
				if (Now_en->spawn->waveEnd)
				{
					if (sound_Start)
					{
						//사운드
						audiocomp->Stop();
						background = LoadObject<USoundCue>(NULL, TEXT("SoundCue'/Game/AI_Project/AI_Pakage/BaseAI/Sound/NonCombatting_Cue.NonCombatting_Cue'"));
						audiocomp->SetSound(background);
						audiocomp->Play();
						audiocomp->FadeIn(2.0f);
						sound_Start = false;
					}
					
					ListVoidReset();
				}
			}
			else
			{
				if (sound_Start)
				{
					//사운드
					audiocomp->Stop();
					background = LoadObject<USoundCue>(NULL, TEXT("SoundCue'/Game/AI_Project/AI_Pakage/BaseAI/Sound/NonCombatting_Cue.NonCombatting_Cue'"));
					audiocomp->SetSound(background);
					audiocomp->Play();
					audiocomp->FadeIn(2.0f);
					sound_Start = false;
				}
				ListVoidReset();
				
				
			}
		}
	}
}


void AAICommander::ListVoidReset()
{
	for (auto ai : List_Division)
	{
		AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ai.Key)->GetController());
		if (AIController == nullptr)
		{
			continue;
		}
		if (AIController->GetBlackboardComponent() == NULL)
		{
			continue;
		}
		AIController->GetBlackboardComponent()->SetValueAsBool("AI_Active", false);
	}
	List_Division.Reset();
	List_Location.Reset();
	List_Suppression.Reset();
	List_CoverPoint.Reset();
	Sup_Array.Reset();
	AddIndex = 0;
	MapList_Start = false;
	Blackboard->SetValueAsBool("CmdAI_Active", false);
	Blackboard->SetValueAsObject("Cmd_Target", NULL);

	
}

void AAICommander::ListAdd(AActor* ac)
{
	List_Division.Add(ac, AddIndex);
	List_Location.Add(AddIndex, ac->GetActorLocation());
	List_Suppression.Add(AddIndex, 0.0f);
	List_CoverPoint.Add(AddIndex, FVector(0, 0, 0));
	AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ac)->GetController());
	if (AIController)
	{
		if (AIController->GetBlackboardComponent())
		{
			AIController->GetBlackboardComponent()->SetValueAsBool("AI_Active", true);
			AIController->GetBlackboardComponent()->SetValueAsInt("ID_Number", AddIndex);
		}
	}
	AddIndex++;
}


void AAICommander::ListStartSet(const AEncounterSpace* en)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, sub->GetName());
	for (auto& subAi : en->AIArray)
	{
		List_Division.Add(subAi, AddIndex);
		List_Location.Add(AddIndex, subAi->GetActorLocation());
		List_Suppression.Add(AddIndex, 0.0f);
		List_CoverPoint.Add(AddIndex, FVector(0,0,0));
		Blackboard->SetValueAsBool("CmdAI_Active", true);

		AIController = Cast<AAI_Controller>(Cast<AAICharacter>(subAi)->GetController());
		if (AIController == nullptr)
		{
			continue;
		}
		if (AIController->GetBlackboardComponent() == NULL)
		{
			continue;
		}
		if (AIController->btree)
		{
			AIController->RunBTT();
		}
		AIController->GetBlackboardComponent()->SetValueAsInt("ID_Number", AddIndex);
		AIController->GetBlackboardComponent()->SetValueAsBool("AI_Active", true);
		AddIndex++;
	}
	
	MapList_Start = true;
}

void AAICommander::ListTickSet()
{
	SightIn_CHK = false;
	for (auto ai : List_Division)
	{
		if (ai.Key != nullptr)
		{
			List_Location.Add(ai.Value, ai.Key->GetActorLocation());
			AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ai.Key)->GetController());
			if (AIController == nullptr)
			{
				continue;
			}
			if (AIController->GetBlackboardComponent() == NULL)
			{
				continue;
			}
			if (AIController->commander != this)
			{
				AIController->commander = this;
			}
			List_Suppression.Add(ai.Value, AIController->GetBlackboardComponent()->GetValueAsFloat("Sup_TotalPoint"));
			if (s_time >= Cast<AAICharacter>(ai.Key)->sup_sharetime)
			{
				SuppressionShare();

				s_time = 0;
			}
			if (AIController)
			{
				if (AIController->GetBlackboardComponent())
				{
					if (AIController->GetBlackboardComponent()->GetValueAsBool("Sight_In"))
					{
						SightIn_CHK = true;
					}
				}
			}
		}
		
	}
	if (SightIn_CHK == false)
	{
		Cmd_SightOut = true;
	}
	else
	{
		Cmd_SightOut = false;
	}
}

void AAICommander::TargetTickSet()
{
	if (List_Division.IsEmpty())
	{
		return;
	}
	for (auto& en_Ai : List_Division)
	{
		AIController = Cast<AAI_Controller>(Cast<AAICharacter>(en_Ai.Key)->GetController());
		if (AIController == nullptr)
		{
			continue;
		}
		if (AIController->GetBlackboardComponent() == NULL)
		{
			continue;
		}
		if (AIController->GetBlackboardComponent()->GetValueAsBool("Simple_Run") == false)
		{
			if (AIController->GetBlackboardComponent()->GetValueAsObject("Target") == nullptr)
			{
				continue;
			}
			Blackboard->SetValueAsObject("Cmd_Target", AIController->GetBlackboardComponent()->GetValueAsObject("Target"));

			for (auto& ai : List_Division)
			{
				if (ai == en_Ai)
				{
					continue;
				}
				if (ai.Key == nullptr)
				{
					continue;
				}
				if (Cast<AAICharacter>(ai.Key)->GetController() == nullptr)
				{
					continue;
				}
				AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ai.Key)->GetController());
				if (AIController == nullptr)
				{
					continue;
				}
				if (AIController->GetBlackboardComponent() == NULL)
				{
					continue;
				}
				if (AIController->GetBlackboardComponent()->GetValueAsBool("Simple_Run") == false)
				{
					if (AIController->GetBlackboardComponent()->GetValueAsObject("Target") == nullptr)
					{
						AIController->GetBlackboardComponent()->SetValueAsObject("Target", Blackboard->GetValueAsObject("Cmd_Target"));
					}
				}

			}
		}
		
	}
}

void AAICommander::SuppressionShare()
{
	List_Suppression.ValueSort([](float a, float b) {
		return a > b;
		});
	Sup_Array.Reset();
	List_Suppression.GenerateValueArray(Sup_Array);
	MaxSupLoc = *List_Location.Find(*List_Suppression.FindKey(Sup_Array[0]));
	for (auto ac : List_Division)
	{
		AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ac.Key)->GetController());
		if (AIController == nullptr)
		{
			continue;
		}
		if (AIController->GetBlackboardComponent() == NULL)
		{
			continue;
		}
		if (*List_Location.Find(ac.Value) != MaxSupLoc)
		{
			sup_value = AIController->GetBlackboardComponent()->GetValueAsFloat("Sup_TotalPoint");
			sup_value += (Sup_Array[0] / 15)
				* (1 - ((FVector::Distance(MaxSupLoc, *List_Location.Find(ac.Value))) / Cast<AAICharacter>(ac.Key)->sup_sharerange));
			if (sup_value >= Sup_Array[0])
			{
				sup_value = Sup_Array[0];
			}
			AIController->GetBlackboardComponent()->SetValueAsFloat("Sup_TotalPoint", sup_value);
		}
	}
}

void AAICommander::CoverPointEn(const AEncounterSpace* en)
{
	CoverEnArray.Reset();
	if (CoverArray.IsEmpty())
	{
		return;
	}
	for (auto cover : CoverArray)
	{
		if ((en->GetActorLocation().X - en->CollisionMesh->GetScaledBoxExtent().X) <= cover.X
			&& (en->GetActorLocation().X + en->CollisionMesh->GetScaledBoxExtent().X) >= cover.X)
		{
			if ((en->GetActorLocation().Y - en->CollisionMesh->GetScaledBoxExtent().Y) <= cover.Y
				&& (en->GetActorLocation().Y + en->CollisionMesh->GetScaledBoxExtent().Y) >= cover.Y)
			{
				if ((en->GetActorLocation().Z - en->CollisionMesh->GetScaledBoxExtent().Z) <= cover.Z
					&& (en->GetActorLocation().Z + en->CollisionMesh->GetScaledBoxExtent().Z) >= cover.Z)
				{
					CoverEnArray.Add(cover);
				}
			}
		}
	}
}

void AAICommander::CoverPointEnemy()
{
	CoverEnemyArray.Reset();
	if (CoverArray.IsEmpty())
	{
		return;
	}
	if (CoverEnArray.IsEmpty())
	{
		return;
	}
	for (auto subencover : CoverEnArray)
	{
		FCollisionQueryParams collisionParams;
		collisionParams.AddIgnoredActor(this);
		if (GetWorld()->LineTraceSingleByChannel(result, subencover, player->GetActorLocation(), ECC_Visibility, collisionParams))
		{
			if (result.GetActor()->ActorHasTag("Cover"))
			{
				if (FVector::Distance(subencover, result.ImpactPoint) < 100.0f)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, subencover.ToString());
					//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, result.ImpactPoint.ToString());
					if (FVector::Distance(subencover, player->GetActorLocation()) >= 300.0f)
					{
						CoverEnemyArray.Add(subencover);
					}
				}
			}
		}
	}
}

void AAICommander::SiegeCoverPoint()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, "SiegeCoverPoint");
	SiegeCoverArray.Reset();
	if (CoverArray.IsEmpty())
	{
		return;
	}
	if (CoverEnArray.IsEmpty())
	{
		return;
	}
	if (CoverEnemyArray.IsEmpty())
	{
		return;
	}
	for (auto enemy_cover : CoverEnemyArray)
	{
		if (FVector::Distance(player->GetActorLocation(), enemy_cover) < siege_range)
		{
			SiegeCoverArray.Add(enemy_cover);
		}
	}

}

void AAICommander::DetourCoverPoint()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, player->GetCapsuleComponent()->GetForwardVector().ToString());//GetSocketLocation("pelvis"));
	DetourCoverArray.Reset();
	if (CoverArray.IsEmpty())
	{
		return;
	}
	if (CoverEnArray.IsEmpty())
	{
		return;
	}
	if (CoverEnemyArray.IsEmpty())
	{
		return;
	}
	for (auto enemy_cover : CoverEnemyArray)
	{
		FVector Find_rot = UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), enemy_cover).Vector();
		Find_rot.Normalize();
		float Dot_Cover = FVector::DotProduct(player->GetCapsuleComponent()->GetForwardVector(), Find_rot);
		float angle = FMath::RadiansToDegrees(FMath::Acos(Dot_Cover));
		if (angle < detour_angle && angle > ndetour_angle)
		{
			if (FVector::Distance(player->GetActorLocation(), enemy_cover) < detour_range)
			{
				DetourCoverArray.Add(enemy_cover);
			}
		}
	}	
}


bool AAICommander::IsPlayerInsideFanArea(FVector CoverPoint,float LocationRadius, float FanAngle, FVector FanDirection)
{
	FVector playerLocation = player->GetActorLocation();
	FVector locationToPlayer = playerLocation - CoverPoint;
	float AngleToPlayer = FMath::Acos(FVector::DotProduct(FanDirection, locationToPlayer.GetSafeNormal()));

	if (AngleToPlayer <= FMath::DegreesToRadians(FanAngle) * 0.5f && locationToPlayer.Size2D() <= LocationRadius)
	{
		return true;
	}

	return false;
}

bool AAICommander::IsCoverInsideFanArea(FVector CoverPoint, float FanAngle, FVector FanDirection)
{
	FVector playerLocation = player->GetActorLocation();
	FVector locationToPlayer = playerLocation - CoverPoint;
	float AngleToPlayer = FMath::Acos(FVector::DotProduct(FanDirection, locationToPlayer.GetSafeNormal()));

	if (AngleToPlayer <= FMath::DegreesToRadians(FanAngle) * 0.5f)
	{
		return true;
	}

	return false;
}

FVector AAICommander::OptimumPoint(const FVector FinalLocation, const AActor* AI_Actor, const FVector MiddleLocation)
{
	FVector move_Loc;
	FVector player_rot = UKismetMathLibrary::FindLookAtRotation(AI_Actor->GetActorLocation(), player->GetActorLocation()).Vector();
	FVector Find_rot = UKismetMathLibrary::FindLookAtRotation(AI_Actor->GetActorLocation(), FinalLocation).Vector();
	float DI_Loc = 1 / 10;
	float DI_Ang = 1 / 120;
	float TotalPoint = 0.0f;
	float MaxPoint = 0.0f;
	
	FVector cross_Final = FVector::CrossProduct(player_rot, FinalLocation);
	if (FinalLocation != FVector::ZeroVector)
	{
		if (MiddleLocation != FVector::ZeroVector)
		{
			if (FVector::Distance(MiddleLocation, AI_Actor->GetActorLocation()) >= 100)
			{
				return MiddleLocation;
			}
			for (auto C_Point : CoverEnemyArray)
			{
				FVector cover_rot = UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), C_Point).Vector();
				cover_rot.Normalize();
				float Dot_Cover = FVector::DotProduct(Find_rot, cover_rot);
				float angle = FMath::RadiansToDegrees(FMath::Acos(Dot_Cover));
				float AngPoint = 100 - (100 * (angle * DI_Ang));
				if (cross_Final.Z > 0) // Right
				{
					if (FVector::CrossProduct(player_rot, C_Point).Z > 0)
					{
						if (MiddleLocation != C_Point)
						{
							if (AngPoint < 0)
							{
								AngPoint = 0;
							}
							float DisPoint = 100 - (FVector::Distance(AI_Actor->GetActorLocation(), C_Point) * DI_Loc);
							if (DisPoint < 0)
							{
								DisPoint = 0;
							}
							TotalPoint = AngPoint + DisPoint;
							if (MaxPoint == 0)
							{
								MaxPoint = TotalPoint;
								move_Loc = C_Point;
							}
							else
							{
								if (MaxPoint <= TotalPoint)
								{
									MaxPoint = TotalPoint;
									move_Loc = C_Point;
								}
							}
						}
					}
				}
				else if (cross_Final.Z <= 0) //Left
				{
					if (FVector::CrossProduct(player_rot, C_Point).Z <= 0)
					{
						if (MiddleLocation != C_Point)
						{
							if (AngPoint < 0)
							{
								AngPoint = 0;
							}
							float DisPoint = 100 - (FVector::Distance(AI_Actor->GetActorLocation(), C_Point) * DI_Loc);
							if (DisPoint < 0)
							{
								DisPoint = 0;
							}
							TotalPoint = AngPoint + DisPoint;
							if (MaxPoint == 0)
							{
								MaxPoint = TotalPoint;
								move_Loc = C_Point;
							}
							else
							{
								if (MaxPoint <= TotalPoint)
								{
									MaxPoint = TotalPoint;
									move_Loc = C_Point;
								}
							}
						}
					}
				}
				//FVector crossPrdt = FVector::CrossProduct(forwardVect, A);
			}
		}
		else
		{
			for (auto C_Point : CoverEnemyArray)
			{
				FVector cover_rot = UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), C_Point).Vector();
				cover_rot.Normalize();
				float Dot_Cover = FVector::DotProduct(Find_rot, cover_rot);
				float angle = FMath::RadiansToDegrees(FMath::Acos(Dot_Cover));
				float AngPoint = 100 - (100 * (angle * DI_Ang));
				if (cross_Final.Z > 0) // Right
				{
					if (FVector::CrossProduct(player_rot, C_Point).Z > 0)
					{
						if (AngPoint < 0)
						{
							AngPoint = 0;
						}
						float DisPoint = 100 - (FVector::Distance(AI_Actor->GetActorLocation(), C_Point) * DI_Loc);
						if (DisPoint < 0)
						{
							DisPoint = 0;
						}
						TotalPoint = AngPoint + DisPoint;
						if (MaxPoint == 0)
						{
							MaxPoint = TotalPoint;
							move_Loc = C_Point;
						}
						else
						{
							if (MaxPoint <= TotalPoint)
							{
								MaxPoint = TotalPoint;
								move_Loc = C_Point;
							}
						}
					}
				}
				else if (cross_Final.Z <= 0) // Left
				{
					if (FVector::CrossProduct(player_rot, C_Point).Z <= 0)
					{
						if (AngPoint < 0)
						{
							AngPoint = 0;
						}
						float DisPoint = 100 - (FVector::Distance(AI_Actor->GetActorLocation(), C_Point) * DI_Loc);
						if (DisPoint < 0)
						{
							DisPoint = 0;
						}
						TotalPoint = AngPoint + DisPoint;
						if (MaxPoint == 0)
						{
							MaxPoint = TotalPoint;
							move_Loc = C_Point;
						}
						else
						{
							if (MaxPoint <= TotalPoint)
							{
								MaxPoint = TotalPoint;
								move_Loc = C_Point;
							}
						}
					}
				}
				//FVector crossPrdt = FVector::CrossProduct(forwardVect, A);
			}
		}
	}
	
	return move_Loc;
}

bool AAICommander::SameDetourPoint(FVector FinalLocation, FVector MiddleLocation)
{
	if (FinalLocation == MiddleLocation)
	{
		return true;
	}
	return false;
}

void AAICommander::ZeroListCoverPoint(const AActor* AIActor)
{
	List_CoverPoint.Add(*List_Division.Find(AIActor), FVector::ZeroVector);
}




