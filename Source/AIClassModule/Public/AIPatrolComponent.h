// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AICharacter.h"
#include "PatrolActor.h"
#include "AIPatrolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AICLASSMODULE_API UAIPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIPatrolComponent();

	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolSetting")
		bool patrol_State;

	// ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolSetting")
		APatrolActor* patrol_Actor;

	// AI ĳ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolSetting")
		AAICharacter* owner;

	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolSetting")
		TArray<FVector> patrol_Point;
	// ���� ���� ��ȣ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolSetting")
		int patrol_Num;
	// ���� ���� ( true : ������, false : ������)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolSetting")
		bool patrol_Dir;

	// ������ �ַ��� ����
	bool check;
	float t;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// ��ǥ�� �̵�
	void PatrolMove(const FVector Destination);

	// ���� Ȱ/��Ȱ��ȭ
	UFUNCTION(BlueprintCallable, Category = "Patrol")
		void PatrolEnable(bool p_flag);

	// ���� ����
	void PatrolControl();
};
