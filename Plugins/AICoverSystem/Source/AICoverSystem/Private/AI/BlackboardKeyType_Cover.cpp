//// Copyright, (c) Sami Kangasmaa 2022


#include "AI/BlackboardKeyType_Cover.h"
#include "AITypes.h"

const UBlackboardKeyType_Cover::FDataType UBlackboardKeyType_Cover::InvalidValue = FCover();

UBlackboardKeyType_Cover::UBlackboardKeyType_Cover(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ValueSize = sizeof(FCover);
	SupportedOp = EBlackboardKeyOperation::Basic;
}

FCover UBlackboardKeyType_Cover::GetValue(const UBlackboardKeyType_Cover* KeyOb, const uint8* RawData)
{
	return GetValueFromMemory<FCover>(RawData);
}

bool UBlackboardKeyType_Cover::SetValue(UBlackboardKeyType_Cover* KeyOb, uint8* RawData, const FCover& Value)
{
	return SetValueInMemory<FCover>(RawData, Value);
}

EBlackboardCompare::Type UBlackboardKeyType_Cover::CompareValues(const UBlackboardComponent& OwnerComp, const uint8* MemoryBlock,
	const UBlackboardKeyType* OtherKeyOb, const uint8* OtherMemoryBlock) const
{
	const FCover MyValue = GetValue(this, MemoryBlock);
	const FCover OtherValue = GetValue((UBlackboardKeyType_Cover*)OtherKeyOb, OtherMemoryBlock);

	return MyValue.Equals(OtherValue) ? EBlackboardCompare::Equal : EBlackboardCompare::NotEqual;
}

void UBlackboardKeyType_Cover::Clear(UBlackboardComponent& OwnerComp, uint8* RawData)
{
	SetValueInMemory<FCover>(RawData, FCover());
}

bool UBlackboardKeyType_Cover::IsEmpty(const UBlackboardComponent& OwnerComp, const uint8* RawData) const
{
	const FCover Cover = GetValue(this, RawData);
	return !Cover.IsValid();
}

FString UBlackboardKeyType_Cover::DescribeValue(const UBlackboardComponent& OwnerComp, const uint8* RawData) const
{
	const FCover Cover = GetValue(this, RawData);
	return Cover.IsValid() ? Cover.Data.Location.ToString() : TEXT("(invalid)");
}

bool UBlackboardKeyType_Cover::GetLocation(const UBlackboardComponent& OwnerComp, const uint8* RawData, FVector& Location) const
{
	const FCover Cover = GetValue(this, RawData);
	Location = Cover.Data.Location;
	return Cover.IsValid();
}

void UBlackboardKeyType_Cover::InitializeMemory(UBlackboardComponent& OwnerComp, uint8* RawData)
{
	SetValue(this, RawData, FCover());
}

bool UBlackboardKeyType_Cover::TestBasicOperation(const UBlackboardComponent& OwnerComp, const uint8* MemoryBlock, EBasicKeyOperation::Type Op) const
{
	const FCover Cover = GetValue(this, MemoryBlock);
	return (Op == EBasicKeyOperation::Set) ? Cover.IsValid() : !Cover.IsValid();
}

