// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_OrderWaitChk.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API UBTT_OrderWaitChk : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_OrderWaitChk();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_CoverPossiblePoint)
		UBlackboardComponent* BlackboardComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_CoverPossiblePoint)
		class AActor* commander;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_CoverPossiblePoint)
		class AAI_Controller* AIController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_CoverPossiblePoint)
		class AAICharacter* ACharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_CoverPossiblePoint)
		bool orderchk;
};
