// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPatrolComponent.h"
#include "Components/WidgetComponent.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include "Math/Vector.h"
#include "PatrolActor.h"

UAIPatrolComponent::UAIPatrolComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	patrol_Dir = true;
}


void UAIPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

	owner = Cast<AAICharacter>(GetOwner());
	patrol_Num = patrol_Actor->patrol_Point.Num() - 1;
}

void UAIPatrolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAIPatrolComponent::PatrolMove(const FVector Destination)
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(owner->GetController(), Destination);
}

void UAIPatrolComponent::SetNextPatrolNum()
{
	// 방향 체크 후 정/역방향 이동
	switch (patrol_Type)
	{
	case Patrol_Type::STAY:
		// 가마니
		break;
	case Patrol_Type::CYCLE:
		cur_patrol_Num++;
		// 끝 번호까지 갔을 경우
		if (cur_patrol_Num > patrol_Num)
		{
			cur_patrol_Num = 0;
		}
		break;
	case Patrol_Type::RETURN:
		if (patrol_Dir)
		{
			cur_patrol_Num++;
			// 끝 번호까지 갔을 경우
			if (cur_patrol_Num >= patrol_Num)
			{
				patrol_Dir = !patrol_Dir;
			}
		}
		else if (!patrol_Dir)
		{
			cur_patrol_Num--;
			// 첫 번호로 다시 왔을 경우
			if (cur_patrol_Num == 0)
			{
				patrol_Dir = !patrol_Dir;
			}
		}
	}
}


