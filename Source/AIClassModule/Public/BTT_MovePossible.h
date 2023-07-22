// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MovePossible.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API UBTT_MovePossible : public UBTTaskNode
{
	GENERATED_BODY()
	

public:
	UBTT_MovePossible();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_MovePossible)
		UBlackboardComponent* BlackboardComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_MovePossible)
		class AAI_Controller* AIController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_MovePossible)
		class AAICharacter* ACharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_MovePossible)
		class AActor* commander;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_MovePossible)
		float dist;

};
