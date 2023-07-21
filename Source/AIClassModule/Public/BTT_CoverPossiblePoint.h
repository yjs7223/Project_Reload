// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_CoverPossiblePoint.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API UBTT_CoverPossiblePoint : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_CoverPossiblePoint();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_CoverPossiblePoint)
		class AActor* commander;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_CoverPossiblePoint)
		class AAI_Controller* AIController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_CoverPossiblePoint)
		bool coverpossible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_CoverPossiblePoint)
		float mindis;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_CoverPossiblePoint)
		FVector mindislocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_CoverPossiblePoint)
		bool same;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_CoverPossiblePoint)
		bool arraysame;
};
