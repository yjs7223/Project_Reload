// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_PlayerInCoverChk.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API UBTS_PlayerInCoverChk : public UBTService
{
	GENERATED_BODY()
public:
	UBTS_PlayerInCoverChk();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTS_SupportState)
		UBlackboardComponent* BlackboardComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTS_SupportState)
		class AAICommander* AIController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTS_SupportState)
		class AAICharacter* ACharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTS_SupportState)
		class AActor* commander;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BTS_SupportState)
		class ACharacter* player;
};
