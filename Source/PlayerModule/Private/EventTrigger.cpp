// Fill out your copyright notice in the Description page of Project Settings.


#include "EventTrigger.h"
#include "Components/BoxComponent.h"

// Sets default values
AEventTrigger::AEventTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerEventEnum = ETriggerEventEnum::TE_NoneEvent;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;
	selectNum = 0;
	timeOutCount = 0;
}

// Called when the game starts or when spawned
void AEventTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEventTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEventTrigger::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (OtherActor->Tags.Num() > 0)
		{
			if (OtherActor->ActorHasTag("Player"))
			{
				switch (TriggerEventEnum)
				{
				case ETriggerEventEnum::TE_NoneEvent:
					//이벤트 없음
					break;
				case ETriggerEventEnum::TE_DialogueEvent:
					//대사 출력
					PlayDialogueEvent(selectNum);
					break;
				case ETriggerEventEnum::TE_MissionEvent:
					//미션 출력
					PlayMissoinEvent(selectNum);
					break;
				case ETriggerEventEnum::TE_OpenlevelEvent:
					//페이드 아웃 후 레벨 오픈
					OpenLevelEvent(selectNum);
					break;
				case ETriggerEventEnum::TE_TimeOutEvent:
					//제한시간 시작
					PlayTimeOutEvent();
					break;
				default:
					break;
				}
			}
		}
	}
}

void AEventTrigger::PlayDialogueEvent(int p_selectNum)
{


}

void AEventTrigger::PlayMissoinEvent(int p_selectNum)
{

}

void AEventTrigger::OpenLevelEvent(int p_selectNum)
{

}

void AEventTrigger::PlayTimeOutEvent()
{

}

