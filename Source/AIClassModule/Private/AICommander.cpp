// Fill out your copyright notice in the Description page of Project Settings.



#include "AICommander.h"
#include "AICharacter.h"
#include "AIStatComponent.h"
#include "EncounterSpace.h"
#include "SubEncounterSpace.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "ST_Suppression.h"
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
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"




// Sets default values
AAICommander::AAICommander()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//encounter = CreateDefaultSubobject<AEncounterSpace>(TEXT("Encounter"));
	PrimaryActorTick.bCanEverTick = true;
	AddIndex = 0;
	MapList_Start = false;
	sightin = false;
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SuppressionDataObject(TEXT("DataTable'/Game/Aws/AI_Stat/DT_Suppression.DT_Suppression'"));
	if (DT_SuppressionDataObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
		DT_Suppression = DT_SuppressionDataObject.Object;
	}
	//BT
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/JHB/BT_AICommander.BT_AICommander'"));
	if (BTObject.Succeeded())
	{
		btree = BTObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("BehaviorTree Succeed!"));
	}
	behavior_tree_component = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB_AICommanderObject(TEXT("BlackBoard'/Game/JHB/BB_Commander.BB_Commander'"));
	if (BB_AICommanderObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("AICommander Blackboard Succeed!"));
		BB_AICommander = BB_AICommanderObject.Object;
	}
	AddIndex = 0;
	enemycover = false;
	/*static ConstructorHelpers::FObjectFinder<AAIController> BaseAI_Ctr_Object(TEXT("AIController'/Game/JHB/BaseAI_Ctr.BaseAI_Ctr'"));
	if (BaseAI_Ctr_Object.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
		BaseAI_Ctr = BaseAI_Ctr_Object.Object;
	}*/
	SightIn_CHK = false;
	Patrol_CHK = false;
	Cmd_SightOut = false;
	
	SetDataTable("Rifle_E");
}

void AAICommander::OnPossess(APawn* pPawn)
{
	Super::OnPossess(pPawn);
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
void AAICommander::TargetTickSet(ASubEncounterSpace* sub)
{
	for (auto& subAi : Cast<ASubEncounterSpace>(sub)->AIArray)
	{
		AIController = nullptr;
		ACharacter = Cast<AAICharacter>(subAi);
		if (ACharacter)
		{
			AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ACharacter)->GetController());
		}
		if (AIController)
		{
			if (AIController->BlackboardComponent)
			{
				BlackboardComponent = AIController->BlackboardComponent;
				if (BlackboardComponent->GetValueAsBool("Sight_In"))
				{
					Blackboard->SetValueAsObject("Cmd_Target", BlackboardComponent->GetValueAsObject("Target"));
					for (auto& ai : Cast<ASubEncounterSpace>(sub)->AIArray)
					{
						if (ai != subAi)
						{
							AIController = nullptr;
							ACharacter = Cast<AAICharacter>(ai);
							if (ACharacter)
							{
								AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ACharacter)->GetController());
							}
							if (AIController)
							{
								if (AIController->BlackboardComponent)
								{
									BlackboardComponent = AIController->BlackboardComponent;
									if (BlackboardComponent->GetValueAsObject("Target") == nullptr)
									{
										BlackboardComponent->SetValueAsObject("Target", Blackboard->GetValueAsObject("Cmd_Target"));
									}
								}
							}
						}
					}
				}
				else
				{
					sightin = false;
					for (auto& aichk : Cast<ASubEncounterSpace>(sub)->AIArray)
					{
						if (aichk != subAi)
						{
							AIController = nullptr;
							ACharacter = Cast<AAICharacter>(aichk);
							if (ACharacter)
							{
								AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ACharacter)->GetController());
							}
							if (AIController)
							{
								if (AIController->BlackboardComponent)
								{
									BlackboardComponent = AIController->BlackboardComponent;
									if (BlackboardComponent->GetValueAsBool("Sight_In"))
									{
										sightin = true;
									}
								}
							}
						}
					}
					if (sightin == false)
					{
						for (auto& sightai : Cast<ASubEncounterSpace>(sub)->AIArray)
						{
							AIController = nullptr;
							ACharacter = Cast<AAICharacter>(sightai);
							if (ACharacter)
							{
								AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ACharacter)->GetController());
							}
							if (AIController)
							{
								if (AIController->BlackboardComponent)
								{
									BlackboardComponent = AIController->BlackboardComponent;
									BlackboardComponent->SetValueAsBool("Sight_In", false);
								}
							}
						}
					}
				}
			}
		}
	}
}
// Called when the game starts or when spawned
void AAICommander::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEncounterSpace::StaticClass(), EncounterArray);
}

// Called every frame
void AAICommander::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	s_time += DeltaTime;
	
	ListSet();
	UseBlackboard(BB_AICommander, BlackboardComponent);
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
					Blackboard->SetValueAsBool("CmdAI_Active", true);
					if (!MapList_Start)
					{
						ListStartSet(Cast<ASubEncounterSpace>(sub));
					}
					else
					{
						ListTickSet(Cast<ASubEncounterSpace>(sub), Cast<AEncounterSpace>(en));
						TargetTickSet(Cast<ASubEncounterSpace>(sub));
						CoverPointSubEn(Cast<ASubEncounterSpace>(sub));
						CoverPointEnemy();
						SiegeCoverPoint();
						DetourCoverPoint();
					}
				}
			}
		}
		
	}
}

void AAICommander::ListReset(ASubEncounterSpace* sub)
{
	List_Division.Reset();
	List_RDivision.Reset();
	List_Combat.Reset();
	List_Location.Reset();
	List_Suppression.Reset();
}

void AAICommander::ListStartSet(ASubEncounterSpace* sub)
{
	for (auto& subAi : Cast<ASubEncounterSpace>(sub)->AIArray)
	{
		
		List_Division.Add(subAi, AddIndex);
		List_RDivision.Add(AddIndex, subAi);
		List_Combat.Add(AddIndex, ECombat::Patrol);
		List_Location.Add(AddIndex, subAi->GetActorLocation());
		List_Suppression.Add(AddIndex, 0.0f);
		List_CoverPoint.Add(AddIndex, FVector(0,0,0));
		
		AIController = nullptr;
		ACharacter = Cast<AAICharacter>(subAi);
		if (ACharacter)
		{
			AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ACharacter)->GetController());
		}
		if (AIController)
		{
			if (AIController->BlackboardComponent)
			{
				BlackboardComponent = AIController->BlackboardComponent;
				BlackboardComponent->SetValueAsBool("AI_Active", true);
				BlackboardComponent->SetValueAsInt("ID_Number", AddIndex);
				BlackboardComponent->SetValueAsEnum("Combat", (uint8)*List_Combat.Find(AddIndex));
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::FromInt(AIController->BlackboardComponent->GetValueAsEnum("Combat")));
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
			ACharacter = Cast<AAICharacter>(ai);
			if (ACharacter)
			{
				AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ai)->GetController());
			}
			if (AIController)
			{
				if (AIController->commander != this)
				{
					AIController->commander = this;
				}
				if (AIController->BlackboardComponent)
				{
					BlackboardComponent = AIController->BlackboardComponent;
					List_Suppression.Add(*FindActor, AIController->BlackboardComponent->GetValueAsFloat("Sup_TotalPoint"));
					
					if (s_time >= sup_sharetime)
					{
						SuppressionShare(sub);
						
						s_time = 0;
					}
					if (BlackboardComponent->GetValueAsBool("Sight_In"))
					{
						SightIn_CHK = true;
					}
				}
			}
		}
		else
		{
			if (Cast<ASubEncounterSpace>(sub)->LevelActive == false)
			{
				subenNum = Cast<ASubEncounterSpace>(sub)->LevelNum + 1;
				for (auto suben : Cast<AEncounterSpace>(en)->LevelArray)
				{
					if (Cast<ASubEncounterSpace>(suben)->LevelNum == subenNum)
					{
						Cast<ASubEncounterSpace>(suben)->LevelActive = true;
					}
				}
			}

		}
	}
	if (SightIn_CHK == false)
	{
		Patrol_CHK = false;
		for (auto enemy : List_Division)
		{
			AIController = nullptr;
			ACharacter = Cast<AAICharacter>(enemy.Key);
			if (ACharacter)
			{
				AIController = Cast<AAI_Controller>(Cast<AAICharacter>(enemy.Key)->GetController());
			}
			if (AIController)
			{
				if (AIController->BlackboardComponent)
				{
					BlackboardComponent = AIController->BlackboardComponent;
					if (BlackboardComponent->GetValueAsEnum("Combat") == 0)
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
			ACharacter = Cast<AAICharacter>(ac);
			if (ACharacter)
			{
				AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ac)->GetController());
			}
			if (AIController)
			{
				if (AIController->BlackboardComponent)
				{
					BlackboardComponent = AIController->BlackboardComponent;
					if (*List_Location.Find(*FindAc) != MaxSupLoc)
					{
						sup_value = BlackboardComponent->GetValueAsFloat("Sup_TotalPoint");
						sup_value += (Sup_Array[0] / 5)
							* (1 - ((FVector::Distance(MaxSupLoc, *List_Location.Find(*FindAc))) / sup_sharerange));
						if (sup_value >= Sup_Array[0])
						{
							sup_value = Sup_Array[0];
						}
						BlackboardComponent->SetValueAsFloat
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
			if ((sub->GetActorLocation().X - sub->CollisionMesh->GetScaledBoxExtent().X) <= cover.X)
			{
				if ((sub->GetActorLocation().X + sub->CollisionMesh->GetScaledBoxExtent().X) >= cover.X)
				{
					if ((sub->GetActorLocation().Y - sub->CollisionMesh->GetScaledBoxExtent().Y) <= cover.Y)
					{
						if ((sub->GetActorLocation().Y + sub->CollisionMesh->GetScaledBoxExtent().Y) >= cover.Y)
						{
							if ((sub->GetActorLocation().Z - sub->CollisionMesh->GetScaledBoxExtent().Z) <= cover.Z)
							{
								if ((sub->GetActorLocation().Z + sub->CollisionMesh->GetScaledBoxExtent().Z) >= cover.Z)
								{
									CoverSubEnArray.Add(cover);
								}
							}
							
						}
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
				if (GetWorld()->LineTraceSingleByChannel(result, subencover, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation(), ECC_Visibility, collisionParams))
				{
					enemycover = false;

					if (result.GetActor()->ActorHasTag("Cover"))
					{
						if (FVector::Distance(subencover, result.ImpactPoint) < 90.0f)
						{
							//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, subencover.ToString());
							//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, result.ImpactPoint.ToString());
							if (FVector::Distance(subencover, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()) >= 500.0f)
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
	SiegeCoverArray.Reset();
	if (!CoverArray.IsEmpty())
	{
		if (!CoverSubEnArray.IsEmpty())
		{
			if (!CoverEnemyArray.IsEmpty())
			{
				for (auto enemy_cover : CoverEnemyArray)
				{
					if (IsPlayerInsideFanArea(enemy_cover, 1000, 360, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorForwardVector()))
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
	DetourCoverArray.Reset();
	if (!CoverArray.IsEmpty())
	{
		if (!CoverSubEnArray.IsEmpty())
		{
			if (!CoverEnemyArray.IsEmpty())
			{
				for (auto enemy_cover : CoverEnemyArray)
				{
					if (IsPlayerInsideFanArea(enemy_cover, 2000, 240, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorForwardVector()))
					{
						if (!IsPlayerInsideFanArea(enemy_cover, 2000, 160, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorForwardVector()))
						{
							DetourCoverArray.Add(enemy_cover);
						}
					}
				}
			}
		}
	}
}

bool AAICommander::IsPlayerInsideFanArea(FVector CoverPoint,float LocationRadius, float FanAngle, FVector FanDirection)
{
	FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FVector locationToPlayer = playerLocation - CoverPoint;

	float AngleToPlayer = FMath::Acos(FVector::DotProduct(FanDirection, locationToPlayer.GetSafeNormal()));

	if (AngleToPlayer <= FMath::DegreesToRadians(FanAngle) / 2.0f && locationToPlayer.Size2D() <= LocationRadius)
	{
		return true;
	}

	return false;
}




