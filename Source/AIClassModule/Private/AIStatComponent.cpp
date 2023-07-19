// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStatComponent.h"
#include "AICommander.h"
#include "Components/CapsuleComponent.h"
#include "ST_Suppression.h"
#include "AICharacter.h"
#include "AIController.h"
#include "AI_Controller.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

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
	PlayerAtt_ai = false;
	//AICommander = AAICommander::aicinstance;
}

void UAIStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Time += DeltaTime;

	if (PlayerAtt_ai)
	{
		SuppresionPoint();
	}
	else
	{
		if (Time >= sup_DecTime)
		{
			AIController = nullptr;
			ACharacter = Cast<AAICharacter>(GetOwner());
			if (ACharacter)
			{
				AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ACharacter)->GetController());
			}
			if (AIController)
			{
				if (AIController->BlackboardComponent)
				{
					BlackboardComponent = AIController->BlackboardComponent;

					sup_total = BlackboardComponent->GetValueAsFloat("Sup_TotalPoint");
					sup_total -= sup_DecPoint;
					if (sup_total <= 0)
					{
						sup_total = 0;
					}
					BlackboardComponent->SetValueAsFloat("Sup_TotalPoint", sup_total);
					
				}
			}
			Time = 0;
		}
		
	}
}

void UAIStatComponent::Attacked(float p_damage)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("itkikik"));
	sup_Input = p_damage * sup_DecInput;
	Time = 0;
	PlayerAtt_ai = true;
	
}

void UAIStatComponent::Attacked(float p_damage, FHitResult result)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("ckck"));
	curHP -= p_damage;
	if (curHP < 0.0f)
	{
		curHP = 0.0f;
	}
	sup_Input = p_damage;
	Time = 0;
	PlayerAtt_ai = true;
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
	AI_PlayerDis = GetOwner()->GetDistanceTo(GetWorld()->GetFirstPlayerController()->GetPawn());
	AIController = nullptr;
	ACharacter = Cast<AAICharacter>(GetOwner());
	if (ACharacter)
	{
		AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ACharacter)->GetController());
	}
	if (AIController)
	{
		if (AIController->BlackboardComponent)
		{
			BlackboardComponent = AIController->BlackboardComponent;
			if (Cast<AAI_Controller>(Cast<AAICharacter>(GetOwner())->GetController())->UseBlackboard(AIController->BBAsset, BlackboardComponent))
			{
				switch (BlackboardComponent->GetValueAsEnum("Combat"))
				{
				case 0:
					sup_middlePoint = (1 - (AI_PlayerDis / sup_MaxRange)) * 1.2;
					break;
				case 1:
				case 2:
					sup_middlePoint = (1 - (AI_PlayerDis / sup_MaxRange)) * 1;
					break;
				case 3:
					sup_middlePoint = (1 - (AI_PlayerDis / sup_MaxRange)) * 0.7;
					break;
				case 4:
					sup_middlePoint = (1 - (AI_PlayerDis / sup_MaxRange)) * 0.5;
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
				BlackboardComponent->SetValueAsFloat("Sup_TotalPoint", sup_total);
				Time = 0.0f;
				PlayerAtt_ai = false;
			}
			
		}
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
}
