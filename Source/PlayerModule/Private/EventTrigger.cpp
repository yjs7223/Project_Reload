// Fill out your copyright notice in the Description page of Project Settings.


#include "EventTrigger.h"
#include "Components/BoxComponent.h"
#include "DialogueWidget.h"
#include "MissionWidget.h"
#include "PlayerCharacter.h"
#include "PlayerHUDWidget.h"
#include "DialogueWidget.h"
#include "TimeOutWidget.h"
#include "Kismet/GameplayStatics.h"
//#include ""

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
	
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AEventTrigger::OnBoxBeginOverlap);
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
				APlayerCharacter* pc = Cast<APlayerCharacter>(OtherActor);
				switch (TriggerEventEnum)
				{
				case ETriggerEventEnum::TE_NoneEvent:
					//이벤트 없음
					break;
				case ETriggerEventEnum::TE_DialogueEvent:
					//대사 출력
					PlayDialogueEvent(pc, selectNum);
					break;
				case ETriggerEventEnum::TE_MissionEvent:
					//미션 출력
					PlayMissoinEvent(pc, selectNum);
					break;
				case ETriggerEventEnum::TE_OpenlevelEvent:
					//페이드 아웃 후 레벨 오픈
					OpenLevelEvent(pc, selectNum);
					break;
				case ETriggerEventEnum::TE_TimeOutEvent:
					//제한시간 시작
					PlayTimeOutEvent(pc, timeOutCount);
					break;
				default:
					break;
				}
			}
		}
	}

	Destroy();
}

void AEventTrigger::PlayDialogueEvent(APlayerCharacter* player, int p_selectNum)
{
	if (UDialogueWidget* DialogueWidget = player->PlayerHUDWidget->DialogueWidget)
	{
		DialogueWidget->SetDialogueText(FName(FString::FromInt(p_selectNum)));
	}
}

void AEventTrigger::PlayMissoinEvent(APlayerCharacter* player, int p_selectNum)
{
	if (UMissionWidget* MissionWidget = player->PlayerHUDWidget->MissionWidget)
	{
		MissionWidget->SetMissionText(FName(FString::FromInt(p_selectNum)));
	}
}

void AEventTrigger::OpenLevelEvent(APlayerCharacter* player, int p_selectNum)
{
	player->PlayerHUDWidget->PlayFadeInOutAnim(false);

	FTimerHandle openLevelHandle;

	GetWorld()->GetTimerManager().SetTimer(openLevelHandle,
		FTimerDelegate::CreateLambda([&]()
			{
				switch (p_selectNum)
				{
				case 0:
					UGameplayStatics::OpenLevel(GetWorld(), FName("Target_level"));
					break;
				case 1:
					UGameplayStatics::OpenLevel(GetWorld(), FName("AI_Test_Level"));
					break;
				case 2:
					UGameplayStatics::OpenLevel(GetWorld(), FName("AI_Test_Level"));
					break;
				case 3:
					UGameplayStatics::OpenLevel(GetWorld(), FName("AI_Test_Level"));
					break;
				default:
					UGameplayStatics::OpenLevel(GetWorld(), FName("AI_Test_Level"));
					break;
				}
			}
	), 2.0f, false);
	
}

void AEventTrigger::PlayTimeOutEvent(APlayerCharacter* player, float p_timeOutCount)
{
	if (UTimeOutWidget* TimeOutWidget = player->PlayerHUDWidget->TimeOutWidget)
	{
		TimeOutWidget->StartCount(p_timeOutCount);
	}
}

