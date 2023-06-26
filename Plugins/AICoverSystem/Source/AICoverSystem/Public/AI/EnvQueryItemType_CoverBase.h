//// Copyright, (c) Sami Kangasmaa 2022

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "CoverSystemPublicData.h"
#include "EnvQueryItemType_CoverBase.generated.h"

struct FBlackboardKeySelector;
class UBlackboardComponent;

/**
 * Type of an item in env query system to query and test covers
 */
UCLASS()
class AICOVERSYSTEM_API UEnvQueryItemType_CoverBase : public UEnvQueryItemType_VectorBase
{
	GENERATED_UCLASS_BODY()

public:
	typedef FCover FValueType;

	static FCover GetValue(const uint8* RawData);
	static void SetValue(uint8* RawData, const FCover& Value);

	virtual FVector GetItemLocation(const uint8* RawData) const override;

	virtual void AddBlackboardFilters(FBlackboardKeySelector& KeySelector, UObject* FilterOwner) const override;
	virtual bool StoreInBlackboard(FBlackboardKeySelector& KeySelector, UBlackboardComponent* Blackboard, const uint8* RawData) const override;
	virtual FString GetDescription(const uint8* RawData) const override;
	
};
