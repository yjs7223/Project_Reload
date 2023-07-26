// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacterMoveComponent.h"
#include "ST_Move.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AICharacter.h"

// Sets default values for this component's properties
UAICharacterMoveComponent::UAICharacterMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Move_Speed = 500.0f;
	timeDeltaTime = 0.0;
	lerpDeltaTime = 0.0;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_MoveDataObject(TEXT("DataTable'/Game/Aws/AI_Stat/DT_Move.DT_Move'"));
	if (DT_MoveDataObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
		DT_Move = DT_MoveDataObject.Object;
	}
	// ...
}


// Called when the game starts
void UAICharacterMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	aicharacter = GetOwner<AAICharacter>();
	SetEnemy("Rifle_E");

	Move_Normal = true;
	// ...
	
}


// Called every frame
void UAICharacterMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Time += DeltaTime;
	if (Move_Normal)//�Ϲݰȱ��϶�
	{
		if (Move_Attack)
		{
			if (Move_Hit)
			{
				if (Time >= 1.0)
				{
					Move_Hit = false;
				}
				timeDeltaTime += DeltaTime;
				if (timeDeltaTime >= m_ParallelTime)
				{
					timeDeltaTime = m_ParallelTime;
				}
				lerpDeltaTime = timeDeltaTime * 0.5;
				Move_Speed = FMath::Lerp(100, m_SpdHit, lerpDeltaTime);
				aicharacter->GetCharacterMovement()->MaxWalkSpeed = Move_Speed;
			}
			else
			{
				timeDeltaTime += DeltaTime;
				if (timeDeltaTime >= m_ParallelTime)
				{
					timeDeltaTime = m_ParallelTime;
				}
				lerpDeltaTime = timeDeltaTime * 0.5;
				Move_Speed = FMath::Lerp(100, m_SpdAttack, lerpDeltaTime);
				aicharacter->GetCharacterMovement()->MaxWalkSpeed = Move_Speed;
			}
			
		}
		else if (Move_Hit)
		{
			timeDeltaTime += DeltaTime;
			if (timeDeltaTime >= m_ParallelTime)
			{
				timeDeltaTime = m_ParallelTime;
			}
			lerpDeltaTime = timeDeltaTime * 0.5;
			Move_Speed = FMath::Lerp(100, m_SpdHit, lerpDeltaTime);
			aicharacter->GetCharacterMovement()->MaxWalkSpeed = Move_Speed;
		}
		else
		{
			timeDeltaTime += DeltaTime;
			if (timeDeltaTime >= m_ChangeTime)
			{
				timeDeltaTime = m_ChangeTime;
			}
			lerpDeltaTime = timeDeltaTime * 0.2;
			Move_Speed = FMath::Lerp(100, m_SpdNomal, lerpDeltaTime);
			aicharacter->GetCharacterMovement()->MaxWalkSpeed = Move_Speed;
		}
	}
	else
	{
		Move_Speed = 100;
		timeDeltaTime = 0;
		lerpDeltaTime = 0;
		aicharacter->GetCharacterMovement()->MaxWalkSpeed = Move_Speed;
	}
	
	// ...
}

void UAICharacterMoveComponent::SetEnemy(FName EnemyName)
{
	FST_Move* MoveData = DT_Move->FindRow<FST_Move>(EnemyName, FString(""));
	if (MoveData)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData Succeed!"));

		m_ChangeTime = MoveData->Move_ChangeTime;
		m_ParallelTime = MoveData->Move_ParallelTime;
		m_SpdNomal = MoveData->Spd_Normal;
		m_SpdAttack = MoveData->Spd_Attack;
		m_SpdHit = MoveData->Spd_Hit;
	}
}
