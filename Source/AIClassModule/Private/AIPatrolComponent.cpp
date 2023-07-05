// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPatrolComponent.h"
#include "Components/WidgetComponent.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include "Math/Vector.h"
#include "PatrolActor.h"

UAIPatrolComponent::UAIPatrolComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	patrol_State = true;
	patrol_Dir = true;
}


// Called when the game starts
void UAIPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

	owner = Cast<AAICharacter>(GetOwner());
	for (int i = 0; i < patrol_Actor->patrol_Point.Num(); i++)
	{
		patrol_Point.Add(GetOwner()->GetActorTransform().TransformPosition(patrol_Actor->patrol_Point[i]));
	}
}


// Called every frame
void UAIPatrolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PatrolControl();
}

void UAIPatrolComponent::PatrolMove(const FVector Destination)
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(owner->GetController(), Destination);
}

void UAIPatrolComponent::PatrolEnable(bool p_flag)
{
	patrol_State = p_flag;
}

void UAIPatrolComponent::PatrolControl()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("PatrolControl")));
	// 순찰 상태가 아니면 취소
	if (!patrol_State)
	{
		return;
	}

	// 배열이 비었으면 순찰없음
	if (patrol_Point.Num() != 0)
	{
		// 목적지 도착하면 다시 출발
		if (FVector::Distance(owner->GetActorLocation(), patrol_Point[patrol_Num]) <= 100)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Goal")));
			if (!patrol_State)
			{
				// 방향 체크 후 정/역방향 이동
				if (patrol_Dir)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Dir = true")));
					// 끝 번호까지 갔을 경우
					if (patrol_Num >= patrol_Point.Num())
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Max")));
						patrol_Dir = !patrol_Dir;
					}
					else
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Num++")));
						patrol_Num++;
					}
				}
				else if (!patrol_Dir)
				{
					// 첫 번호로 다시 왔을 경우
					if (patrol_Num == 0)
					{
						patrol_Dir = !patrol_Dir;
					}
					else
					{
						patrol_Num--;
					}
				}
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("patrol_State = true")));
				patrol_State = true;
			}
		}

		if (patrol_State)
		{
			// 다음 순찰 위치로 정방향 이동
			PatrolMove(patrol_Point[patrol_Num]);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Name : %s"), *patrol_Point[patrol_Num].ToString()));
			patrol_State = false;
		}

	}

}


