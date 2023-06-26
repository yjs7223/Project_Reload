//// Copyright, (c) Sami Kangasmaa 2022


#include "AI/Tests/EnvQueryTest_ProvidesCover.h"

#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "AI/EnvQueryItemType_CoverBase.h"
#include "CollisionQueryParams.h"
#include "Engine/World.h"
#include "CoverSolver.h"

#define LOCTEXT_NAMESPACE "EnvQueryGenerator"

UEnvQueryTest_ProvidesCover::UEnvQueryTest_ProvidesCover()
{
	Cost = EEnvTestCost::High;
	ValidItemType = UEnvQueryItemType_CoverBase::StaticClass();
	SetWorkOnFloatValues(false);

	BoolValue.DefaultValue = true;
	Context = UEnvQueryContext_Querier::StaticClass();
}

void UEnvQueryTest_ProvidesCover::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();

	BoolValue.BindData(QueryOwner, QueryInstance.QueryID);
	const bool bRequireProvideCover = BoolValue.GetValue();

	TArray<FVector> ContextLocations;
	if (!QueryInstance.PrepareContext(Context, ContextLocations))
	{
		return;
	}
	
	TArray<AActor*> ContextActors;
	QueryInstance.PrepareContext(Context, ContextActors);

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FCover Cover = UEnvQueryItemType_CoverBase::GetValue(QueryInstance.RawData.GetData() + QueryInstance.Items[It.GetIndex()].DataOffset);
		for (int32 Idx = 0; Idx < ContextLocations.Num(); Idx++)
		{
			const bool bProvidesCover = FCoverSolver::TraceToCover(QueryInstance.World, Cover, ContextLocations[Idx], TraceChannel.GetValue(), &ContextActors);
			It.SetScore(TestPurpose, FilterType, bProvidesCover, bRequireProvideCover);
		}
	}
}

FText UEnvQueryTest_ProvidesCover::GetDescriptionTitle() const
{
	UEnum* ChannelEnum = StaticEnum<ECollisionChannel>();
	FString ChannelDesc = ChannelEnum->GetDisplayNameTextByValue(TraceChannel.GetValue()).ToString();

	FString TraceDesc = FString::Printf(TEXT("%s %s"), TEXT("from"), *UEnvQueryTypes::DescribeContext(Context).ToString());

	return FText::FromString(FString::Printf(TEXT("%s: %s on %s"),
		*Super::GetDescriptionTitle().ToString(), *TraceDesc, *ChannelDesc));
}

FText UEnvQueryTest_ProvidesCover::GetDescriptionDetails() const
{
	FString HitDesc = BoolValue.DefaultValue ? FString(TEXT("require provide cover")) : FString(TEXT("require not provide cover"));
	return FText::FromString(FString::Printf(TEXT("%s"), *HitDesc));
}

#undef LOCTEXT_NAMESPACE