//// Copyright, (c) Sami Kangasmaa 2022

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_ParallelToCover.generated.h"

UENUM(BlueprintType)
enum class EEnvTestParallerCover : uint8
{
	Dot3D UMETA(DisplayName = "Dot (3D)"),
	Dot2D UMETA(DisplayName = "Dot 2D (Heading)")
};

/**
 * Tests if cover forward is in parallel with direction vector from cover to context using dot product of the vectors
 * Use this test to filter covers, so the AI can get behind objects (relative to given context such as player)
 */
UCLASS()
class AICOVERSYSTEM_API UEnvQueryTest_ParallelToCover : public UEnvQueryTest
{
	GENERATED_BODY()

public:

	UEnvQueryTest_ParallelToCover();

	/** 
	* Context(s) to check against if the cover is in parallel direction
	* Typically this would be context of the AI's target that it wants to shoot or hide from, such as player character
	*/
	UPROPERTY(EditDefaultsOnly, Category = Test)
	TSubclassOf<UEnvQueryContext> Context;


	UPROPERTY(EditDefaultsOnly, Category = Test)
	EEnvTestParallerCover TestMode = EEnvTestParallerCover::Dot2D;

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
	
	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;

};
