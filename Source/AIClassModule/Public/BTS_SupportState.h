// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_SupportState.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API UBTS_SupportState : public UBTService
{
	GENERATED_BODY()
public:
	UBTS_SupportState();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTS_SupportState)
		FVector Sup_Vec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTS_SupportState)
		bool Dis_start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTS_SupportState)
		float Min_Dis;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTS_SupportState)
		int Min_Dis_Key;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTS_SupportState)
		float Dis;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTS_SupportState)
		float support_default;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTS_SupportState)
		bool DT_On;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTS_SupportState)
		FVector Com_Vec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTS_SupportState)
		class AAI_Controller* aic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AICommander)
		class UDataTable* DT_Suppression;

public:
	UFUNCTION()
		void SetDataTable(FName EnemyName);
};
