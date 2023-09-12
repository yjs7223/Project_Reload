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

	// Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolSetting")
		Patrol_Type patrol_Type;

	// ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolSetting")
		APatrolActor* patrol_Actor;

	// AI ĳ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolSetting")
		AAICharacter* owner;

	// �ִ� ���� ��ȣ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolSetting")
		int patrol_Num;

	// ���� ���� ��ȣ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolSetting")
		int cur_patrol_Num;

	// ���� ���� ( true : ������, false : ������)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolSetting")
		bool patrol_Dir;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// ��ǥ�� �̵�
	void PatrolMove(const FVector Destination);

	// ���� ��ȣ �ѱ��
	UFUNCTION(BlueprintCallable, Category = "Patrol")
	void SetNextPatrolNum();
};
