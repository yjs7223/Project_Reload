// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_DetourCoverSelection.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API UBTT_DetourCoverSelection : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_DetourCoverSelection();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_DetourCoverSelection)
		UBlackboardComponent* BlackboardComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_DetourCoverSelection)
		class AAICommander* commander;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_DetourCoverSelection)
		class AAI_Controller* AIController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_DetourCoverSelection)
		class AAICharacter* ACharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_DetourCoverSelection)
		float Dis_Loc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_DetourCoverSelection)
		bool arraysame;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_DetourCoverSelection)
		bool Detourchange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_DetourCoverSelection)
		class AActor* select_ai;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTT_DetourCoverSelection)
		class ACharacter* player; //cast
};
