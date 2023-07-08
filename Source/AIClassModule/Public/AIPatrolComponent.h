// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AICharacter.h"
#include "PatrolActor.h"
#include "AIPatrolComponent.generated.h"


UENUM(BlueprintType)
enum class Patrol_Type : uint8
{
	STAY, RETURN, CYCLE
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AICLASSMODULE_API UAIPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAIPatrolComponent();

	// 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolSetting")
		Patrol_Type patrol_Type;

	// 액터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolSetting")
		APatrolActor* patrol_Actor;

	// AI 캐릭터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolSetting")
		AAICharacter* owner;

	// 최대 순찰 번호
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolSetting")
		int patrol_Num;

	// 현재 순찰 번호
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolSetting")
		int cur_patrol_Num;

	// 순찰 방향 ( true : 정방향, false : 역방향)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolSetting")
		bool patrol_Dir;

	// 딜레이 주려고 만듦
	bool check;
	float t;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// 목표로 이동
	void PatrolMove(const FVector Destination);

	// 순찰 번호 넘기기
	UFUNCTION(BlueprintCallable, Category = "Patrol")
	void SetNextPatrolNum();
};
