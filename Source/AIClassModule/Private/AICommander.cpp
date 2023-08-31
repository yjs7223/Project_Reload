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

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CommanderDataObject(TEXT("DataTable'/Game/AI_Project/DT/DT_Commander.DT_Commander'"));
	if (DT_CommanderDataObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
		DT_Commander = DT_CommanderDataObject.Object;
	}
	//BT
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/AI_Project/AI_Pakage/Commander/BT/BT_AICommander.BT_AICommander'"));
	if (BTObject.Succeeded())
	{
		btree = BTObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("BehaviorTree Succeed!"));
	}
	behavior_tree_component = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB_AICommanderObject(TEXT("BlackboardData'/Game/AI_Project/AI_Pakage/Commander/BB/BB_Commander.BB_Commander'"));
	if (BB_AICommanderObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("AICommander Blackboard Succeed!"));
		BB_AICommander = BB_AICommanderObject.Object;
	}
	
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

	SetCommanderDataTable("Commander");
}


void AAICommander::SetCommanderDataTable(FName EnemyName)
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
	if (m_en != nullptr)
	{
		if (Now_en != m_en)
		{
			Now_en = m_en;
			m_en = nullptr;
		}
		else
		{
			m_en = nullptr;
		}
	}
	if (m_suben != nullptr)
	{
		if (Now_suben != m_suben)
		{
			ListVoidReset();
			Now_suben = m_suben;
			CoverManager->ChangeEncounter();
			//UAudioComponent* ac = NewObject<UAudioComponent>();
			audiocomp->Stop();
			USoundCue* background = LoadObject<USoundCue>(NULL, TEXT("SoundCue'/Game/yjs/Sounds/S_Fire_Support_LOOP_150bpm_Cue.S_Fire_Support_LOOP_150bpm_Cue'"));
			audiocomp->SetSound(background);
			audiocomp->Play();
			audiocomp->FadeIn(2.0f);

			m_suben = nullptr;
		}
		else
		{
			m_suben = nullptr;
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
	if (En_AIActive)
	{
		AIActive(Now_en);
	}
	if (!Now_suben->LevelActive)
	{
		ListVoidReset();
	}
	
	

	if (Now_suben->spawn)
	{
		Now_suben->spawn->check_Overlap = true;
	}
	if (!MapList_Start)
	{
		ListStartSet(Now_suben);
	}
	else
	{
		ListTickSet(Now_suben, Cast<AEncounterSpace>(Now_en));
		TargetTickSet(Now_suben);
		CoverPointEnemy();
		if (List_Division.Num() <= 0)
		{
			if (Now_suben->spawn != nullptr)
			{
				if (Now_suben->spawn->waveEnd)
				{
					ListReset(Now_suben);
				}
			}
			else
			{
				ListReset(Now_suben);
			}
		}
	}
}
void AAICommander::AIActive(AEncounterSpace* en)
{
	for (auto sub : en->LevelArray)
	{
		ASubEncounterSpace* suben = Cast<ASubEncounterSpace>(sub);

		for (auto ai : suben->M_AIArray)
		{
			AAICharacter* subai = Cast<AAICharacter>(ai);
			AIController = Cast<AAI_Controller>(subai->GetController());
			if (AIController)
			{
				if (AIController->GetBlackboardComponent())
				{
					AIController->GetBlackboardComponent()->SetValueAsBool("AI_Active", true);
				}

				subai->Init();
			}
		}
	}
	En_AIActive = false;
}

void AAICommander::ListReset(ASubEncounterSpace* sub)
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
	//List_Combat.Reset();
	List_Location.Reset();
	List_Suppression.Reset();
	List_CoverPoint.Reset();
	Sup_Array.Reset();
	if (sub->LevelActive)
	{
		sub->LevelActive = false;
	}
	AddIndex = 0;
	MapList_Start = false;
	Blackboard->SetValueAsBool("CmdAI_Active", false);
	Blackboard->SetValueAsObject("Cmd_Target", NULL);


	audiocomp->StopDelayed(2.0f);
	audiocomp->FadeOut(2.0f, 1.0f);
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
	//List_Combat.Reset();
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
	//List_Combat.Add(AddIndex, ECombat::Patrol);
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
			//AIController->GetBlackboardComponent()->SetValueAsEnum("Combat", (uint8)*List_Combat.Find(AddIndex));
		}
	}
	AddIndex++;
}


void AAICommander::ListStartSet(ASubEncounterSpace* sub)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, sub->GetName());
	for (auto& subAi : sub->M_AIArray)
	{
		List_Division.Add(subAi, AddIndex);
		//List_Combat.Add(AddIndex, ECombat::Patrol);
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
		//AIController->GetBlackboardComponent()->SetValueAsEnum("Combat", (uint8)*List_Combat.Find(AddIndex));
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::FromInt(AIController->GetBlackboardComponent()->GetValueAsEnum("Combat")));
		AddIndex++;
	}
	CoverPointSubEn(Now_suben);
	/*if (CoverSubEnArray.IsEmpty())
	{
		MapList_Start = false;
		return;
	}*/
	MapList_Start = true;
}

void AAICommander::ListTickSet(ASubEncounterSpace* sub, AEncounterSpace* en)
{
	SightIn_CHK = false;
	for (auto ai : List_Division)
	{
		auto FindActor = List_Division.Find(ai.Key); 
		
		if (FindActor)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("AICOMMENDER"));
			List_Location.Add(*FindActor, ai.Key->GetActorLocation());
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
			List_Suppression.Add(*FindActor, AIController->GetBlackboardComponent()->GetValueAsFloat("Sup_TotalPoint"));
			if (s_time >= Cast<AAICharacter>(ai.Key)->sup_sharetime)
			{
				SuppressionShare(sub);

				s_time = 0;
			}
			if (AIController->GetBlackboardComponent()->GetValueAsBool("Sight_In"))
			{
				SightIn_CHK = true;
			}
		}
		else
		{
			if (sub->LevelActive == false)
			{
				subenNum = sub->LevelNum + 1;
				for (auto suben : en->LevelArray)
				{
					ASubEncounterSpace* sub_en = Cast<ASubEncounterSpace>(suben);
					if (sub_en->LevelNum == subenNum)
					{
						sub_en->LevelActive = true;
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

void AAICommander::TargetTickSet(ASubEncounterSpace* sub)
{
	if (sub->AIArray.IsEmpty())
	{
		return;
	}
	for (auto& subAi : sub->AIArray)
	{
		AIController = Cast<AAI_Controller>(Cast<AAICharacter>(subAi)->GetController());
		if (AIController == nullptr)
		{
			continue;
		}
		if (AIController->GetBlackboardComponent() == NULL)
		{
			continue;
		}
		if (AIController->GetBlackboardComponent()->GetValueAsObject("Target") == nullptr)
		{
			continue;
		}
		Blackboard->SetValueAsObject("Cmd_Target", AIController->GetBlackboardComponent()->GetValueAsObject("Target"));

		for (auto& ai : sub->AIArray)
		{
			if (ai != subAi)
			{
				AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ai)->GetController());
				if (AIController == nullptr)
				{
					continue;
				}
				if (AIController->GetBlackboardComponent() == NULL)
				{
					continue;
				}
				if (AIController->GetBlackboardComponent()->GetValueAsObject("Target") == nullptr)
				{
					AIController->GetBlackboardComponent()->SetValueAsObject("Target", Blackboard->GetValueAsObject("Cmd_Target"));
				}

			}
		}
	}
}

void AAICommander::SuppressionShare(ASubEncounterSpace* sub)
{
	List_Suppression.ValueSort([](float a, float b) {
		return a > b;
		});
	Sup_Array.Reset();
	List_Suppression.GenerateValueArray(Sup_Array);
	MaxSupLoc = *List_Location.Find(*List_Suppression.FindKey(Sup_Array[0]));
	for (auto ac : sub->AIArray)
	{
		auto FindAc = List_Division.Find(ac);
		if (!FindAc)
		{
			continue;
		}
		AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ac)->GetController());
		if (AIController == nullptr)
		{
			continue;
		}
		if (AIController->GetBlackboardComponent() == NULL)
		{
			continue;
		}
		if (*List_Location.Find(*FindAc) != MaxSupLoc)
		{
			sup_value = AIController->GetBlackboardComponent()->GetValueAsFloat("Sup_TotalPoint");
			sup_value += (Sup_Array[0] / 15)
				* (1 - ((FVector::Distance(MaxSupLoc, *List_Location.Find(*FindAc))) / Cast<AAICharacter>(ac)->sup_sharerange));
			if (sup_value >= Sup_Array[0])
			{
				sup_value = Sup_Array[0];
			}
			AIController->GetBlackboardComponent()->SetValueAsFloat("Sup_TotalPoint", sup_value);
		}
	}
}

void AAICommander::CoverPointSubEn(ASubEncounterSpace* sub)
{
	CoverSubEnArray.Reset();
	if (CoverArray.IsEmpty())
	{
		return;
	}
	for (auto cover : CoverArray)
	{
		if ((sub->GetActorLocation().X - sub->CollisionMesh->GetScaledBoxExtent().X) <= cover.X
			&& (sub->GetActorLocation().X + sub->CollisionMesh->GetScaledBoxExtent().X) >= cover.X)
		{
			if ((sub->GetActorLocation().Y - sub->CollisionMesh->GetScaledBoxExtent().Y) <= cover.Y
				&& (sub->GetActorLocation().Y + sub->CollisionMesh->GetScaledBoxExtent().Y) >= cover.Y)
			{
				if ((sub->GetActorLocation().Z - sub->CollisionMesh->GetScaledBoxExtent().Z) <= cover.Z
					&& (sub->GetActorLocation().Z + sub->CollisionMesh->GetScaledBoxExtent().Z) >= cover.Z)
				{
					CoverSubEnArray.Add(cover);
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
	if (CoverSubEnArray.IsEmpty())
	{
		return;
	}
	for (auto subencover : CoverSubEnArray)
	{
		FCollisionQueryParams collisionParams;
		collisionParams.AddIgnoredActor(this);
		if (GetWorld()->LineTraceSingleByChannel(result, subencover, player->GetActorLocation(), ECC_Visibility, collisionParams))
		{

			if (result.GetActor()->ActorHasTag("Cover"))
			{
				if (FVector::Distance(subencover, result.ImpactPoint) < 50.0f)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, subencover.ToString());
					//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, result.ImpactPoint.ToString());
					if (FVector::Distance(subencover, player->GetActorLocation()) >= 500.0f)
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
	if (CoverSubEnArray.IsEmpty())
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
	
	//FVector cover_rot = UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), player->FindComponentByClass<UCoverComponent>()->GetCoverWall()->GetActorLocation()).Vector();
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, player->GetCapsuleComponent()->GetForwardVector().ToString());//GetSocketLocation("pelvis"));
	DetourCoverArray.Reset();
	if (CoverArray.IsEmpty())
	{
		return;
	}
	if (CoverSubEnArray.IsEmpty())
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

FVector AAICommander::OptimumPoint(FVector FinalLocation, AActor* AI_Actor, FVector MiddleLocation)
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
				if (cross_Final.Z > 0) // Right
				{
					if (FVector::CrossProduct(player_rot, C_Point).Z > 0)
					{
						if (MiddleLocation != C_Point)
						{
							FVector cover_rot = UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), C_Point).Vector();
							cover_rot.Normalize();
							float Dot_Cover = FVector::DotProduct(Find_rot, cover_rot);
							float angle = FMath::RadiansToDegrees(FMath::Acos(Dot_Cover));
							float AngPoint = 100 - (100 * (angle * DI_Ang));
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
							FVector cover_rot = UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), C_Point).Vector();
							cover_rot.Normalize();
							float Dot_Cover = FVector::DotProduct(Find_rot, cover_rot);
							float angle = FMath::RadiansToDegrees(FMath::Acos(Dot_Cover));
							float AngPoint = 100 - (100 * (angle * DI_Ang));
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
				if (cross_Final.Z > 0) // Right
				{
					if (FVector::CrossProduct(player_rot, C_Point).Z > 0)
					{
						FVector cover_rot = UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), C_Point).Vector();
						cover_rot.Normalize();
						float Dot_Cover = FVector::DotProduct(Find_rot, cover_rot);
						float angle = FMath::RadiansToDegrees(FMath::Acos(Dot_Cover));
						float AngPoint = 100 - (100 * (angle * DI_Ang));
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
						FVector cover_rot = UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), C_Point).Vector();
						cover_rot.Normalize();
						float Dot_Cover = FVector::DotProduct(Find_rot, cover_rot);
						float angle = FMath::RadiansToDegrees(FMath::Acos(Dot_Cover));
						float AngPoint = 100 - (100 * (angle * DI_Ang));
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

void AAICommander::ZeroListCoverPoint(AActor* AIActor)
{
	List_CoverPoint.Add(*List_Division.Find(AIActor), FVector::ZeroVector);
}




