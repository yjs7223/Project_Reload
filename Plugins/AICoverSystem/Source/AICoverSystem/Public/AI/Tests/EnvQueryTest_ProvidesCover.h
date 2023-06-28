//// Copyright, (c) Sami Kangasmaa 2022

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_ProvidesCover.generated.h"

/**
 * Tests if cover item is currently giving cover from given contex(s)
 */
UCLASS()
class AICOVERSYSTEM_API UEnvQueryTest_ProvidesCover : public UEnvQueryTest
{
	GENERATED_BODY()

public:

	UEnvQueryTest_ProvidesCover();

	// Trace channel to use for testing if there's blocking geometry between cover and context
	UPROPERTY(EditDefaultsOnly, Category = Trace)
	TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_Visibility;

	// Context which we are looking for protection from
	UPROPERTY(EditDefaultsOnly, Category = Trace)
	TSubclassOf<UEnvQueryContext> Context;

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
};
