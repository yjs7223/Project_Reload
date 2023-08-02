//// Copyright, (c) Sami Kangasmaa 2022

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_FreeCover.generated.h"

/**
 * Tests that the cover item is not currently occupied
 */
UCLASS()
class AICOVERSYSTEM_API UEnvQueryTest_FreeCover : public UEnvQueryTest
{
	GENERATED_BODY()
	
public:

	UEnvQueryTest_FreeCover();

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionDetails() const override;

protected:

	// Consider the cover unoccupied, if the querier is occupying it
	UPROPERTY(EditAnywhere, Category = "Test")
	bool bFreeOnSelfOccupy = true;
};
