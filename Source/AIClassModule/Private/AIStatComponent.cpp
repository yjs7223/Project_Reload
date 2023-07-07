// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStatComponent.h"
#include "AICommander.h"
#include "Components/CapsuleComponent.h"
#include "ST_Suppression.h"
#include "AICharacter.h"

UAIStatComponent::UAIStatComponent()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SuppressionDataObject(TEXT("DataTable'/Game/Aws/AI_Stat/DT_Suppression.DT_Suppression'"));
	if (DT_SuppressionDataObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
		DT_Suppression = DT_SuppressionDataObject.Object;
	}
	SetDataTable("Rifle_E");
}

void UAIStatComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner<AAICharacter>();
}

void UAIStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Time += DeltaTime;

	if (PlayerAtt_ai)
	{
		if (Time >= sup_DelayTime)
		{
			SuppresionPoint();
			if (sup_total >= sup_MaxPoint)
			{
				sup_total = sup_MaxPoint;
			}
			Time = 0.0f;
		}
		PlayerAtt_ai = false;
	}
	
}

void UAIStatComponent::Attacked1(float p_damage, FHitResult result)
{
	if (result.GetActor()->ActorHasTag("Enemy"))
	{

		if (result.GetActor()->FindComponentByClass<UCapsuleComponent>() == Cast<AAICharacter>(IndirectCollision)->CollisionMesh)
		{
			
			if (result.GetActor()->FindComponentByClass<UCapsuleComponent>() == Cast<AAICharacter>(IndirectCollision)->GetRootComponent())
			{
				curHP -= p_damage;
				if (curHP < 0.0f)
				{
					curHP = 0.0f;
				}
				sup_Input = p_damage;
				PlayerAtt_ai = true;
			}
			else
			{
				sup_Input = p_damage * sup_DecInput;
			}
		}
	}
}

void UAIStatComponent::SuppresionPoint()
{
	AI_PlayerDis = GetOwner()->GetDistanceTo(GetWorld()->GetFirstPlayerController()->GetPawn());
	switch (*Cast<AAICommander>(AICommander)->List_Combat.Find(*Cast<AAICommander>(AICommander)->List_Division.Find(GetOwner())))
	{
	case ECombat::Patrol:
		sup_middlePoint = (1 - (AI_PlayerDis / sup_MaxRange)) * 1.2;
		break;
	case ECombat::Move:
		sup_middlePoint = (1 - (AI_PlayerDis / sup_MaxRange)) * 1;
		break;
	case ECombat::Attack:
		sup_middlePoint = (1 - (AI_PlayerDis / sup_MaxRange)) * 0.7;
		break;
	case ECombat::InCover:
		sup_middlePoint = (1 - (AI_PlayerDis / sup_MaxRange)) * 0.5;
		break;
	}

	sup_middlePoint *= sup_Multi;
	sup_total = sup_Input * sup_middlePoint;
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
	}
}
