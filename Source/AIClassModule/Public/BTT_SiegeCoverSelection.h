// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SiegeCoverSelection.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API UBTT_SiegeCoverSelection : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_SiegeCoverSelection();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_SiegeCoverSelection)
		UBlackboardComponent* BlackboardComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_SiegeCoverSelection)
		class AAICommander* commander;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_SiegeCoverSelection)
		class AAI_Controller* AIController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_SiegeCoverSelection)
		class AAICharacter* ACharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_SiegeCoverSelection)
		int maplistnum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_SiegeCoverSelection)
		int enemyActivenum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_SiegeCoverSelection)
		FVector movepoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_SiegeCoverSelection)
		bool arraysame;



};
