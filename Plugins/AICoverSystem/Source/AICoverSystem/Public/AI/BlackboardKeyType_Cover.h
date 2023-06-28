//// Copyright, (c) Sami Kangasmaa 2022

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "CoverSystemPublicData.h"
#include "BlackboardKeyType_Cover.generated.h"

class UBlackboardComponent;

/**
 * Custom BB key type to store covers into AI blackboard
 */
UCLASS(EditInlineNew, meta = (DisplayName = "Cover"))
class AICOVERSYSTEM_API UBlackboardKeyType_Cover : public UBlackboardKeyType
{
	GENERATED_UCLASS_BODY()

	typedef FCover FDataType;
	static const FDataType InvalidValue;

	static FCover GetValue(const UBlackboardKeyType_Cover* KeyOb, const uint8* RawData);
	static bool SetValue(UBlackboardKeyType_Cover* KeyOb, uint8* RawData, const FCover& Value);

	virtual EBlackboardCompare::Type CompareValues(const UBlackboardComponent& OwnerComp, const uint8* MemoryBlock,
	const UBlackboardKeyType* OtherKeyOb, const uint8* OtherMemoryBlock) const override;

protected:
	virtual void InitializeMemory(UBlackboardComponent& OwnerComp, uint8* RawData) override;
	virtual FString DescribeValue(const UBlackboardComponent& OwnerComp, const uint8* RawData) const override;
	virtual bool GetLocation(const UBlackboardComponent& OwnerComp, const uint8* RawData, FVector& Location) const override;
	virtual bool IsEmpty(const UBlackboardComponent& OwnerComp, const uint8* MemoryBlock) const override;
	virtual void Clear(UBlackboardComponent& OwnerComp, uint8* MemoryBlock) override;
	virtual bool TestBasicOperation(const UBlackboardComponent& OwnerComp, const uint8* MemoryBlock, EBasicKeyOperation::Type Op) const override;
};
