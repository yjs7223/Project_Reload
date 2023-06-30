// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class AICLASSMODULE_API AMyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMyAIController();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* DT_Range;

private:
	UPROPERTY(transient)
		UBlackboardComponent* BlackboardComp;
};
