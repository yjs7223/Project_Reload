// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStatComponent.h"
#include "AICommander.h"
#include "Components/CapsuleComponent.h"
#include "ST_Suppression.h"
#include "ST_AIShot.h"
#include "AICharacter.h"
#include "AIController.h"
#include "SubEncounterSpace.h"
#include "AI_Controller.h"
#include "AICharacterMoveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

UAIStatComponent::UAIStatComponent()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SuppressionDataObject(TEXT("DataTable'/Game/AI_Project/DT/DT_Suppression.DT_Suppression'"));
	if (DT_SuppressionDataObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
		DT_Suppression = DT_SuppressionDataObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ShotDataObject(TEXT("DataTable'/Game/AI_Project/DT/DT_AIShot.DT_AIShot'"));
	if (DT_ShotDataObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
		DT_Shot = DT_ShotDataObject.Object;
	}
	SetDataTable("Rifle_E");
}

void UAIStatComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner<AAICharacter>();
	PlayerAtt_ai = false;
	player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AIController = Cast<AAI_Controller>(Cast<AAICharacter>(GetOwner())->GetController());
	DI_SupRange = 1 / sup_MaxRange;
	DI_ShotRange = 1 / (shot_MaxRange - shot_MinRange);
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
			if (sup_total <= 0)
			{
				sup_total = 0;
			}
			AIController->GetBlackboardComponent()->SetValueAsFloat("Sup_TotalPoint", sup_total);
		}
		
		Time = 0;
	}
}

void UAIStatComponent::Attacked(float p_damage)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("itkikik"));
	float dis = FVector::Distance(owner->GetActorLocation(), player->GetActorLocation());
	float dmg = (shot_MaxDmg - shot_MinDmg) * (1 - (dis - shot_MinRange) * DI_ShotRange) + shot_MinDmg;
	sup_Input = dmg * sup_DecInput;
	Time = 0;
	PlayerAtt_ai = true;
	SuppresionPoint();
}

void UAIStatComponent::Attacked(float p_damage, FHitResult result)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("ckck"));
	UAICharacterMoveComponent* moveoncmp = owner->FindComponentByClass<UAICharacterMoveComponent>();
	moveoncmp->e_move = EMove::Hit;
	moveoncmp->Time = 0;
	float dis = FVector::Distance(owner->GetActorLocation(), player->GetActorLocation());
	float dmg = (shot_MaxDmg - shot_MinDmg) * (1 - (dis - shot_MinRange) * DI_ShotRange) + shot_MinDmg;
	curHP -= dmg;
	if (curHP < 0.0f)
	{
		curHP = 0.0f;
		isDie = true;
		if (AIController->commander->List_Division.Find(GetOwner()))
		{
			AIController->commander->List_Division.Remove(GetOwner());
		}
	}
	sup_Input = dmg;
	Time = 0;
	PlayerAtt_ai = true;
	SuppresionPoint();
}

//void UAIStatComponent::Attacked(FHitResult result)
//{
//	
//		
//
//}

void UAIStatComponent::SuppresionPoint()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("ckck"));
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
		sup_total += sup_Input * sup_middlePoint;
		//SuppresionPoint();
		if (sup_total >= sup_MaxPoint)
		{
			sup_total = sup_MaxPoint;
		}
		AIController->GetBlackboardComponent()->SetValueAsFloat("Sup_TotalPoint", sup_total);
		Time = 0.0f;
		PlayerAtt_ai = false;
	}
}

void UAIStatComponent::SetDataTable(FName EnemyName)
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
		sup_DecPoint = SuppressionData->Sup_DecPoint;
		sup_DecTime = SuppressionData->Sup_DecTime;
	}
	FST_AIShot* ShotData = DT_Shot->FindRow<FST_AIShot>(EnemyName, FString(""));
	if (ShotData)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData Succeed!"));

		shot_MaxRange = ShotData->Shot_MaxRange;
		shot_MinRange = ShotData->Shot_MinRange;
		shot_MaxDmg = ShotData->Shot_MaxDmg;
		shot_MinDmg = ShotData->Shot_MinDmg;
	}
}
