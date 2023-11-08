// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EventTrigger.generated.h"

UENUM(BlueprintType)
enum class ETriggerEventEnum : uint8
{
	TE_NoneEvent UMETA(DisplayName = "NoneEvent"),
	TE_DialogueEvent UMETA(DisplayName = "DialogueEvent"),
	TE_MissionEvent UMETA(DisplayName = "MissionEvent"),
	TE_OpenlevelEvent UMETA(DisplayName = "OpenlevelEvent"),
	TE_TimeOutEvent UMETA(DisplayName = "TimeOutEvent"),
};


UCLASS(Blueprintable)
class PLAYERMODULE_API AEventTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEventTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void PlayDialogueEvent(APlayerCharacter* player,int p_selectNum);

	UFUNCTION(BlueprintCallable)
	void PlayMissoinEvent(APlayerCharacter* player,int p_selectNum);

	UFUNCTION(BlueprintCallable)
	void OpenLevelEvent(APlayerCharacter* player, int p_selectNum);

	UFUNCTION(BlueprintCallable)
	void PlayTimeOutEvent(APlayerCharacter* player,float p_timeOutCount);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETriggerEventEnum TriggerEventEnum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int selectNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int timeOutCount;
};


