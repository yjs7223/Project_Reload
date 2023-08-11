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
	Move_Speed = 100.0f;
	timeDeltaTime = 0.0;
	lerpDeltaTime = 0.0;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_MoveDataObject(TEXT("DataTable'/Game/AI_Project/DT/DT_Move.DT_Move'"));
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
	//SetEnemy("Rifle_E");

	e_move = EMove::Patrol;
	// ...
	
}


// Called every frame
void UAICharacterMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Time += DeltaTime;
	switch (e_move)
	{
	case EMove::Patrol:
		Move_Speed = m_SpdPatrol;
		timeDeltaTime = 0;
		lerpDeltaTime = 0;
		aicharacter->GetCharacterMovement()->MaxWalkSpeed = Move_Speed;
		break;
	case EMove::Normal:
		timeDeltaTime += DeltaTime;
		if (timeDeltaTime >= m_ChangeTime)
		{
			timeDeltaTime = m_ChangeTime;
		}
		lerpDeltaTime = timeDeltaTime * m_NormalMulti;
		Move_Speed = FMath::Lerp(m_SpdPatrol, m_SpdNormal, lerpDeltaTime);
		aicharacter->GetCharacterMovement()->MaxWalkSpeed = Move_Speed;
		break;
	case EMove::Attack:
		timeDeltaTime += DeltaTime;
		if (timeDeltaTime >= m_ParallelTime)
		{
			timeDeltaTime = m_ParallelTime;
		}
		lerpDeltaTime = timeDeltaTime * m_AttackMulti;
		Move_Speed = FMath::Lerp(m_SpdPatrol, m_SpdAttack, lerpDeltaTime);
		aicharacter->GetCharacterMovement()->MaxWalkSpeed = Move_Speed;
		break;
	case EMove::Hit:
		timeDeltaTime += DeltaTime;
		if (Time >= 0.5f)
		{
			timeDeltaTime = 0;
			e_move = EMove::Normal;
			break;
		}
		if (timeDeltaTime >= m_ParallelTime)
		{
			timeDeltaTime = m_ParallelTime;
		}
		lerpDeltaTime = timeDeltaTime * m_HitMulti;
		Move_Speed = FMath::Lerp(m_SpdPatrol, m_SpdHit, lerpDeltaTime);
		aicharacter->GetCharacterMovement()->MaxWalkSpeed = Move_Speed;
		break;
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
		m_HitTime = MoveData->Move_HitTime;
		m_SpdNormal = MoveData->Spd_Normal;
		m_SpdAttack = MoveData->Spd_Attack;
		m_SpdHit = MoveData->Spd_Hit;
		m_SpdPatrol = MoveData->Spd_Patrol;
		m_NormalMulti = MoveData->Multi_NormalTime;
		m_AttackMulti = MoveData->Multi_AttackTime;
		m_HitMulti = MoveData->Multi_HitTime;
	}
}

void UAICharacterMoveComponent::MoveTimeReset()
{
	timeDeltaTime = 0;
}
