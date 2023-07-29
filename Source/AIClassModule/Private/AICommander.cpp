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

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SuppressionDataObject(TEXT("DataTable'/Game/AI_Project/DT/DT_Suppression.DT_Suppression'"));
	if (DT_SuppressionDataObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
		DT_Suppression = DT_SuppressionDataObject.Object;
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
	
	SetDataTable("Rifle_E");
	SetCommanderDataTable("Commander");
	
}

// Called when the game starts or when spawned
void AAICommander::BeginPlay()
{
	Super::BeginPlay();
	player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEncounterSpace::StaticClass(), EncounterArray);
	UBlackboardComponent* BlackboardComp = Blackboard;
	UseBlackboard(BB_AICommander, BlackboardComp);
	RunBehaviorTree(btree);
	behavior_tree_component->StartTree(*btree);
}

void AAICommander::SetDataTable(FName EnemyName)
{
	FST_Suppression* SuppressionData = DT_Suppression->FindRow<FST_Suppression>(EnemyName, FString(""));
	if (SuppressionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData Succeed!"));

		sup_sharerange = SuppressionData->Sup_ShareRange;
		sup_sharetime = SuppressionData->Sup_ShareTime;
	}
	
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
	for (auto& en : EncounterArray)
	{
		if (Cast<AEncounterSpace>(en)->LevelActive)
		{
			Blackboard->SetValueAsObject("Cmd_Space", en);
			for (auto& sub : Cast<AEncounterSpace>(en)->LevelArray)
			{
				if (Cast<ASubEncounterSpace>(sub)->LevelActive)
				{
					m_suben = Cast<ASubEncounterSpace>(sub);
					if (m_suben->spawn)
					{
						m_suben->spawn->check_Overlap = true;
					}
					Blackboard->SetValueAsBool("CmdAI_Active", true);
					if (!MapList_Start)
					{
						ListStartSet(m_suben);
					}
					else
					{
						ListTickSet(m_suben, Cast<AEncounterSpace>(en));
						TargetTickSet(m_suben);
						CoverPointSubEn(m_suben);
						CoverPointEnemy();
						if (List_Division.Num() <= 0)
						{
							ListReset(m_suben);
						}
					}
				}
				
			}
		}
		
	}
}

void AAICommander::ListReset(ASubEncounterSpace* sub)
{
	List_Division.Reset();
	List_Combat.Reset();
	List_Location.Reset();
	List_Suppression.Reset();
	List_CoverPoint.Reset();
	Sup_Array.Reset();
	sub->en->LevelArray.Remove(this);
	sub->LevelActive = false;
	AddIndex = 0;
	MapList_Start = false;
}

void AAICommander::ListAdd(AActor* ac)
{
	List_Division.Add(ac, AddIndex);
	List_Combat.Add(AddIndex, ECombat::Patrol);
	List_Location.Add(AddIndex, ac->GetActorLocation());
	List_Suppression.Add(AddIndex, 0.0f);
	List_CoverPoint.Add(AddIndex, FVector(0, 0, 0));

	AddIndex++;
}

void AAICommander::ListStartSet(ASubEncounterSpace* sub)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, sub->GetName());
	for (auto& subAi : sub->AIArray)
	{
		List_Division.Add(subAi, AddIndex);
		List_Combat.Add(AddIndex, ECombat::Patrol);
		List_Location.Add(AddIndex, subAi->GetActorLocation());
		List_Suppression.Add(AddIndex, 0.0f);
		List_CoverPoint.Add(AddIndex, FVector(0,0,0));
		
		AIController = nullptr;
		AIController = Cast<AAI_Controller>(Cast<AAICharacter>(subAi)->GetController());
		if (AIController)
		{
			if (AIController->GetBlackboardComponent())
			{

				AIController->RunBTT();
				AIController->GetBlackboardComponent()->SetValueAsBool("AI_Active", true);
				AIController->GetBlackboardComponent()->SetValueAsInt("ID_Number", AddIndex);
				AIController->GetBlackboardComponent()->SetValueAsEnum("Combat", (uint8)*List_Combat.Find(AddIndex));
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::FromInt(AIController->GetBlackboardComponent()->GetValueAsEnum("Combat")));
			}
		}

		AddIndex++;
	}
	MapList_Start = true;
}

void AAICommander::ListTickSet(ASubEncounterSpace* sub, AEncounterSpace* en)
{
	SightIn_CHK = false;
	for (auto ai :sub->AIArray)
	{
		auto FindActor = List_Division.Find(ai); 
		
		if (FindActor)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("AICOMMENDER"));
			List_Location.Add(*FindActor, ai->GetActorLocation());
			AIController = nullptr;
			AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ai)->GetController());
			if (AIController)
			{
				if (AIController->commander != this)
				{
					AIController->commander = this;
				}
				if (AIController->GetBlackboardComponent())
				{
					List_Suppression.Add(*FindActor, AIController->GetBlackboardComponent()->GetValueAsFloat("Sup_TotalPoint"));
					
					if (s_time >= sup_sharetime)
					{
						SuppressionShare(sub);
						
						s_time = 0;
					}
					if (AIController->GetBlackboardComponent()->GetValueAsBool("Sight_In"))
					{
						SightIn_CHK = true;
					}
				}
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
		/*Patrol_CHK = false;
		for (auto enemy : List_Division)
		{
			AIController = nullptr;
			AIController = Cast<AAI_Controller>(Cast<AAICharacter>(enemy.Key)->GetController());
			if (AIController)
			{
				if (AIController->GetBlackboardComponent())
				{
					if (AIController->GetBlackboardComponent()->GetValueAsEnum("Combat") == 0)
					{
						Patrol_CHK = true;
					}
				}
			}
		}
		if (Patrol_CHK)
		{
			Cmd_SightOut = true;
		}
		else
		{
			Cmd_SightOut = false;
		}*/
	}
	else
	{
		Cmd_SightOut = false;
	}
}

void AAICommander::TargetTickSet(ASubEncounterSpace* sub)
{
	if (!sub->AIArray.IsEmpty())
	{
		for (auto& subAi : sub->AIArray)
		{
			AIController = nullptr;
			AIController = Cast<AAI_Controller>(Cast<AAICharacter>(subAi)->GetController());
			if (AIController)
			{
				if (AIController->GetBlackboardComponent())
				{
					if (AIController->GetBlackboardComponent()->GetValueAsBool("Sight_In"))
					{
						Blackboard->SetValueAsObject("Cmd_Target", AIController->GetBlackboardComponent()->GetValueAsObject("Target"));

						for (auto& ai : sub->AIArray)
						{
							UAICharacterMoveComponent* moveoncmp = ai->FindComponentByClass<UAICharacterMoveComponent>();
							moveoncmp->Move_Normal = false;
							moveoncmp->Move_Attack = false;
							moveoncmp->Move_Hit = false;
							if (ai != subAi)
							{
								AIController = nullptr;
								AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ai)->GetController());
								if (AIController)
								{
									if (AIController->GetBlackboardComponent())
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
					else
					{
						sightin = false;
						if (!sub->AIArray.IsEmpty())
						{
							for (auto& aichk : sub->AIArray)
							{
								if (aichk != subAi)
								{
									AIController = nullptr;
									AIController = Cast<AAI_Controller>(Cast<AAICharacter>(aichk)->GetController());
									if (AIController)
									{
										if (AIController->GetBlackboardComponent())
										{
											if (AIController->GetBlackboardComponent()->GetValueAsBool("Sight_In"))
											{
												sightin = true;
											}
										}
									}
								}
							}
							if (sightin == false)
							{
								for (auto& sightai : sub->AIArray)
								{
									AIController = nullptr;
									AIController = Cast<AAI_Controller>(Cast<AAICharacter>(sightai)->GetController());
									if (AIController)
									{
										if (AIController->GetBlackboardComponent())
										{
											AIController->GetBlackboardComponent()->SetValueAsBool("Sight_In", false);
										}
									}
								}
							}
						}
					}
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
		if (FindAc)
		{
			AIController = nullptr;
			AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ac)->GetController());
			if (AIController)
			{
				if (AIController->GetBlackboardComponent())
				{
					if (*List_Location.Find(*FindAc) != MaxSupLoc)
					{
						sup_value = AIController->GetBlackboardComponent()->GetValueAsFloat("Sup_TotalPoint");
						sup_value += (Sup_Array[0] / 5)
							* (1 - ((FVector::Distance(MaxSupLoc, *List_Location.Find(*FindAc))) / sup_sharerange));
						if (sup_value >= Sup_Array[0])
						{
							sup_value = Sup_Array[0];
						}
						AIController->GetBlackboardComponent()->SetValueAsFloat
						("Sup_TotalPoint", sup_value);
					}
				}
			}
		}

	}

}

void AAICommander::CoverPointSubEn(ASubEncounterSpace* sub)
{
	CoverSubEnArray.Reset();
	if (!CoverArray.IsEmpty())
	{
		for (auto cover : CoverArray)
		{
			if ((sub->GetActorLocation().X - sub->CollisionMesh->GetScaledBoxExtent().X) <= cover.X && (sub->GetActorLocation().X + sub->CollisionMesh->GetScaledBoxExtent().X) >= cover.X)
			{
				if ((sub->GetActorLocation().Y - sub->CollisionMesh->GetScaledBoxExtent().Y) <= cover.Y && (sub->GetActorLocation().Y + sub->CollisionMesh->GetScaledBoxExtent().Y) >= cover.Y)
				{
					if ((sub->GetActorLocation().Z - sub->CollisionMesh->GetScaledBoxExtent().Z) <= cover.Z && (sub->GetActorLocation().Z + sub->CollisionMesh->GetScaledBoxExtent().Z) >= cover.Z)
					{
						CoverSubEnArray.Add(cover);
					}
				}
			}
		}
	}
}

void AAICommander::CoverPointEnemy()
{
	CoverEnemyArray.Reset();
	if (!CoverArray.IsEmpty())
	{
		if (!CoverSubEnArray.IsEmpty())
		{
			for (auto subencover : CoverSubEnArray)
			{
				FCollisionQueryParams collisionParams;
				collisionParams.AddIgnoredActor(this);
				if (GetWorld()->LineTraceSingleByChannel(result, subencover, player->GetActorLocation(), ECC_Visibility, collisionParams))
				{
					enemycover = false;
					if (result.GetActor()->ActorHasTag("Cover"))
					{
						if (FVector::Distance(subencover, result.ImpactPoint) < 90.0f)
						{
							//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, subencover.ToString());
							//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, result.ImpactPoint.ToString());
							if (FVector::Distance(subencover, player->GetActorLocation()) >= 500.0f)
							{
								enemycover = true;
							}
						}
					}
					if (enemycover)
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

	if (!CoverArray.IsEmpty())
	{
		if (!CoverSubEnArray.IsEmpty())
		{
			if (!CoverEnemyArray.IsEmpty())
			{
				for (auto enemy_cover : CoverEnemyArray)
				{
					if (IsPlayerInsideFanArea(enemy_cover, siege_range, 360, player->GetMesh()->GetForwardVector()))
					{
						SiegeCoverArray.Add(enemy_cover);
					}
				}
			}
		}
	}
}

void AAICommander::DetourCoverPoint()
{
	if (player->FindComponentByClass<UCoverComponent>()->GetCoverWall())
	{
		FVector cover_rot = UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), player->FindComponentByClass<UCoverComponent>()->GetCoverWall()->GetActorLocation()).Vector();
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, player->GetCapsuleComponent()->GetForwardVector().ToString());//GetSocketLocation("pelvis"));
		DetourCoverArray.Reset();
		if (!CoverArray.IsEmpty())
		{
			if (!CoverSubEnArray.IsEmpty())
			{
				if (!CoverEnemyArray.IsEmpty())
				{
					for (auto enemy_cover : CoverEnemyArray)
					{
						if (IsPlayerInsideFanArea(enemy_cover, detour_range, detour_angle, cover_rot))
						{
							if (!IsPlayerInsideFanArea(enemy_cover, detour_range, ndetour_angle, cover_rot))
							{
								DetourCoverArray.Add(enemy_cover);
							}
						}
					}
				}
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
	FVector Find_rot = UKismetMathLibrary::FindLookAtRotation(AI_Actor->GetActorLocation(), FinalLocation).Vector();
	float DI_Loc = 1 / 10;
	float DI_Ang = 1 / 120;
	float TotalPoint = 0.0f;
	float MaxPoint = 0.0f;
	
	FVector cross_Final = FVector::CrossProduct(Find_rot, FinalLocation);
	
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
				if (FVector::CrossProduct(Find_rot, C_Point).Z > 0)
				{
					if (MiddleLocation != C_Point)
					{
						float Dot_Cover = FVector::DotProduct(Find_rot, C_Point);
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
				if (FVector::CrossProduct(Find_rot, C_Point).Z <= 0)
				{
					if (MiddleLocation != C_Point)
					{
						float Dot_Cover = FVector::DotProduct(Find_rot, C_Point);
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
				if (FVector::CrossProduct(Find_rot, C_Point).Z > 0)
				{
					float Dot_Cover = FVector::DotProduct(Find_rot, C_Point);
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
			else if (cross_Final.Z <= 0) // Right
			{
				if (FVector::CrossProduct(Find_rot, C_Point).Z <= 0)
				{
					float Dot_Cover = FVector::DotProduct(Find_rot, C_Point);
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




