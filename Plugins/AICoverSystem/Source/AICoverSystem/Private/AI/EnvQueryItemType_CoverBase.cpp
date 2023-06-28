//// Copyright, (c) Sami Kangasmaa 2022


#include "AI/EnvQueryItemType_CoverBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

#include "AI/BlackboardKeyType_Cover.h"

UEnvQueryItemType_CoverBase::UEnvQueryItemType_CoverBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ValueSize = sizeof(FCover);
}

FCover UEnvQueryItemType_CoverBase::GetValue(const uint8* RawData)
{
	return GetValueFromMemory<FCover>(RawData);
}

void UEnvQueryItemType_CoverBase::SetValue(uint8* RawData, const FCover& Value)
{
	SetValueInMemory<FCover>(RawData, Value);
}

FVector UEnvQueryItemType_CoverBase::GetItemLocation(const uint8* RawData) const
{
	FCover Cover = UEnvQueryItemType_CoverBase::GetValue(RawData);
	return Cover.IsValid() ? Cover.Data.Location : FVector::ZeroVector;
}

void UEnvQueryItemType_CoverBase::AddBlackboardFilters(FBlackboardKeySelector& KeySelector, UObject* FilterOwner) const
{
	Super::AddBlackboardFilters(KeySelector, FilterOwner);

	const FString FilterName = GetClass()->GetFName().ToString() + TEXT("_Cover");
	KeySelector.AllowedTypes.Add(NewObject<UBlackboardKeyType_Cover>(FilterOwner, *FilterName));
}

bool UEnvQueryItemType_CoverBase::StoreInBlackboard(FBlackboardKeySelector& KeySelector, UBlackboardComponent* Blackboard, const uint8* RawData) const
{
	bool bStored = Super::StoreInBlackboard(KeySelector, Blackboard, RawData);

	// Result blackboard key is cover type, store as Cover
	if (!bStored && KeySelector.SelectedKeyType == UBlackboardKeyType_Cover::StaticClass())
	{
		FCover Cover = UEnvQueryItemType_CoverBase::GetValue(RawData);
		if (Cover.IsValid())
		{
			bStored = Blackboard->SetValue<UBlackboardKeyType_Cover>(KeySelector.GetSelectedKeyID(), Cover);
		}
	}

	// Result blackboard key is vector, store only the location of the cover
	if (!bStored && KeySelector.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		FCover Cover = UEnvQueryItemType_CoverBase::GetValue(RawData);
		if (Cover.IsValid())
		{
			bStored = Blackboard->SetValue<UBlackboardKeyType_Vector>(KeySelector.GetSelectedKeyID(), Cover.Data.Location);
		}
	}

	return bStored;
}

FString UEnvQueryItemType_CoverBase::GetDescription(const uint8* RawData) const
{
	FCover Cover = UEnvQueryItemType_CoverBase::GetValue(RawData);
	return Cover.Data.Location.ToString();
}
