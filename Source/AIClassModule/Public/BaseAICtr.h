// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAICtr.generated.h"


/**
 * 
 */



UCLASS()
class AICLASSMODULE_API ABaseAICtr : public AAIController
{
	GENERATED_BODY()

public :
	ABaseAICtr();

protected:

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* pPawn) override;

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaesAICtr)
		class UBehaviorTree* BTAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaesAICtr)
		class UBlackboardData* BBAsset;

	
};
