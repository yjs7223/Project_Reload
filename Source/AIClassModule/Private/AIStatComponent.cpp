// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStatComponent.h"
#include "AICommander.h"
#include "Components/CapsuleComponent.h"
#include "ST_Suppression.h"
#include "AIWeaponData.h"
#include "AIWeaponComponent.h"
#include "ST_AIBaseStat.h"
#include "AICharacter.h"
#include "AIController.h"
#include "SubEncounterSpace.h"
#include "AI_Controller.h"
#include "AICharacterMoveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIZombie.h"

UAIStatComponent::UAIStatComponent()
{
	DT_Suppression = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/AI_Project/DT/DT_Suppression.DT_Suppression'"));
	DT_AIBaseStat = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/AI_Project/DT/DT_AIBaseStat.DT_AIBaseStat'"));
}

void UAIStatComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner<AAICharacter>();
	moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	PlayerAtt_ai = false;
	player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!GetOwner()->ActorHasTag("Zombie"))
	{
		AIController = Cast<AAI_Controller>(Cast<AAICharacter>(GetOwner())->GetController());
	}
	if (AIController != nullptr)
	{
		if (AIController->GetBlackboardComponent() != nullptr)
		{
			AIController->GetBlackboardComponent()->SetValueAsFloat("Sup_TotalPoint", sup_MinPoint);
		}
	}
	
	//SetDataTable("Rifle_E");
	
	//AICommander = AAICommander::aicinstance;
	
	
}

void UAIStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Time += DeltaTime;

	if (Time >= sup_DecTime)
	{
		if (AIController && AIController->GetBlackboardComponent())
		{
			sup_total = AIController->GetBlackboardComponent()->GetValueAsFloat("Sup_TotalPoint");
			sup_total -= sup_DecPoint;
			if (sup_total <= sup_MinPoint)
			{
				sup_total = sup_MinPoint;
			}
			AIController->GetBlackboardComponent()->SetValueAsFloat("Sup_TotalPoint", sup_total);
		}
		
		Time = 0;
	}
}


void UAIStatComponent::IndirectAttacked(float p_Value)
{
	float dis = FVector::Distance(owner->GetActorLocation(), player->GetActorLocation());
	float dmg = p_Value;
	sup_Input = dmg * sup_DecInput;
	Time = 0;
	PlayerAtt_ai = true;
	SuppresionPoint();
}

void UAIStatComponent::Attacked(float p_damage, ABaseCharacter* attacker, EHitType hittype, FVector attackPoint)
{
	if (type != Enemy_Name::SNIPER) {
		bAttacked = true;
	}
	DI_SupRange = 1 / sup_MaxRange;
	
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("itkikik"));
	float total_dmg;
	total_dmg = p_damage - (p_damage * 0.01f) * Def;
	curHP -= total_dmg;
	Def -= (total_dmg * 0.05f);
	if (AIController != nullptr)
	{
		AIController->GetBlackboardComponent()->SetValueAsFloat("AI_HP", curHP);
	}

	if (curHP <= 0.0f)
	{
		curHP = 0.0f;
		bDie = true;
		diePlay.Broadcast();
		if (Cast<AAICharacter>(GetOwner())->GetRootComponent())
		{
			Cast<AAICharacter>(GetOwner())->Dead();
			Cast<AAICharacter>(GetOwner())->GetRootComponent()->DestroyComponent();
		}
		if (AIController->GetBlackboardComponent()->GetValueAsBool("AI_Active") == true)

		{
			if (Cast<AAICharacter>(GetOwner())->GetRootComponent())
			{
				Cast<AAICharacter>(GetOwner())->Dead();
				Cast<AAICharacter>(GetOwner())->GetRootComponent()->DestroyComponent();
			}
			if (AIController->GetBlackboardComponent()->GetValueAsBool("AI_Active") == true)
			{
				AIController->GetBlackboardComponent()->SetValueAsBool("AI_Active", false);
			}
			if (AIController->commander != nullptr)
			{
				if (AIController->commander->List_Division.Find(GetOwner()) != nullptr)
				{
					int aikey = *AIController->commander->List_Division.Find(GetOwner());
					AIController->commander->List_Division.Remove(GetOwner());
					AIController->commander->List_CoverPoint.Remove(aikey);
					AIController->commander->List_Location.Remove(aikey);
					AIController->commander->List_Suppression.Remove(aikey);
				}
			}
		}

	}
	FTimerHandle stuntimer;
	switch (hittype)
	{
	case EHitType::None:
		break;
	case EHitType::Normal:
		moveoncmp->Time = 0;
		if (moveoncmp->e_move != EMove::Stun)
		{
			moveoncmp->e_move = EMove::Hit;
		}
		break;
	case EHitType::Knockback:
		Knockback.Broadcast(attackPoint, bDie);
		break;
	case EHitType::Stun:
		moveoncmp->Time = 0;
		moveoncmp->e_move = EMove::Stun;
		bIsStun = true;

		owner->GetWorldTimerManager().SetTimer(stuntimer,
			[this]()
			{
				bIsStun = false;
			}
		, 3.0f, false);
		break;
	case EHitType::MAX:
		break;
	default:
		break;
	}
	if (Def < 0.0f)
	{
		Def = 0.0f;
	}
	sup_Input = total_dmg;
	Time = 0;
	PlayerAtt_ai = true;
	SuppresionPoint();

	OnVisibleEnemyHPUIDelegate.ExecuteIfBound();
	OnChangeEnemyHPUIDelegate.ExecuteIfBound();
}

void UAIStatComponent::SuppresionPoint()
{
	//
	
	if (AIController && AIController->GetBlackboardComponent())
	{
		AI_PlayerDis = GetOwner()->GetDistanceTo(player);
		switch (AIController->GetBlackboardComponent()->GetValueAsEnum("Combat"))
		{
		default:
			break;
		case 0:
			sup_middlePoint = (1 - (AI_PlayerDis * DI_SupRange)) * 1.2;
			break;
		case 1:
		case 2:
			sup_middlePoint = (1 - (AI_PlayerDis * DI_SupRange)) * 1;
			break;
		case 3:
			sup_middlePoint = (1 - (AI_PlayerDis * DI_SupRange)) * 0.7;
			break;
		case 4:
			sup_middlePoint = (1 - (AI_PlayerDis * DI_SupRange)) * 0.5;
			break;
		}
		if (Time <= sup_DelayTime)
		{
			sup_middlePoint *= sup_Multi;
		}
		else
		{
			PlayerAtt_ai = false;
		}
		sup_total += (sup_Input*0.5f) * sup_middlePoint;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(AI_PlayerDis));
		if (sup_total >= sup_MaxPoint)
		{
			sup_total = sup_MaxPoint;
		}
		AIController->GetBlackboardComponent()->SetValueAsFloat("Sup_TotalPoint", sup_total);
		Time = 0.0f;
		PlayerAtt_ai = false;
	}
}

void UAIStatComponent::SetDataTable(const FName EnemyName)
{
 	FST_Suppression* SuppressionData = DT_Suppression->FindRow<FST_Suppression>(EnemyName, FString(""));
	if (SuppressionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData Succeed!"));

		sup_DecInput = SuppressionData->Sup_DecInput;
		sup_AimPoint = SuppressionData->Sup_AimPoint;
		sup_AimDelay = SuppressionData->Sup_AimDelay;
		sup_MaxRange = SuppressionData->Sup_MaxRange;
		sup_Multi = SuppressionData->Sup_Multi;
		sup_DelayTime = SuppressionData->Sup_DelayTime;
		sup_MaxPoint = SuppressionData->Sup_MaxPoint;
		sup_MinPoint = SuppressionData->Sup_MinPoint;
		sup_DecPoint = SuppressionData->Sup_DecPoint;
		sup_DecTime = SuppressionData->Sup_DecTime;
	}
	FST_AIBaseStat* AIBaseStatData = DT_AIBaseStat->FindRow<FST_AIBaseStat>(EnemyName, FString(""));
	if (AIBaseStatData)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData Succeed!"));
		
		Def = AIBaseStatData->AI_Def;
		HitReactionScale = AIBaseStatData->AI_HitReactionScale;
	}
}
