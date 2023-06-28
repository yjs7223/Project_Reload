//// Copyright, (c) Sami Kangasmaa 2022


#include "AI/Tests/EnvQueryTest_ParallelToCover.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "AI/EnvQueryItemType_CoverBase.h"

#define LOCTEXT_NAMESPACE "EnvQueryGenerator"

UEnvQueryTest_ParallelToCover::UEnvQueryTest_ParallelToCover()
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_CoverBase::StaticClass();
	SetWorkOnFloatValues(true);
	Context = UEnvQueryContext_Querier::StaticClass();

	TestPurpose = EEnvTestPurpose::Filter;
	FilterType = EEnvTestFilterType::Minimum;

	FloatValueMin.DefaultValue = 0.5f;
	FloatValueMax.DefaultValue = 1.0f;
}

void UEnvQueryTest_ParallelToCover::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();

	FloatValueMin.BindData(QueryOwner, QueryInstance.QueryID);
	float MinThresholdValue = FloatValueMin.GetValue();

	FloatValueMax.BindData(QueryOwner, QueryInstance.QueryID);
	float MaxThresholdValue = FloatValueMax.GetValue();

	TArray<FVector> ContextLocations;
	if (!QueryInstance.PrepareContext(Context, ContextLocations))
	{
		return;
	}

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		FCover Cover = UEnvQueryItemType_CoverBase::GetValue(QueryInstance.RawData.GetData() + QueryInstance.Items[It.GetIndex()].DataOffset);
		if (Cover.IsValid())
		{
			const FVector CoverDirection = Cover.Data.GetDirectionToWall();
			const FVector CoverLocation = Cover.Data.Location;

			for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ContextIndex++)
			{
				const FVector ContextLocation = ContextLocations[ContextIndex];
				const FVector DirectionToContext = FVector(ContextLocation - CoverLocation).GetSafeNormal();

				float Dot = 0.0f;
				if (TestMode == EEnvTestParallerCover::Dot3D)
				{
					Dot = FVector::DotProduct(CoverDirection, DirectionToContext);
				}
				else
				{
					Dot = CoverDirection.CosineAngle2D(DirectionToContext);
				}

				It.SetScore(TestPurpose, FilterType, Dot, MinThresholdValue, MaxThresholdValue);
			}
		}
	}
}

FText UEnvQueryTest_ParallelToCover::GetDescriptionTitle() const
{
	FString Desc = FString::Printf(TEXT("%s"), *UEnvQueryTypes::DescribeContext(Context).ToString());
	return FText::FromString(FString::Printf(TEXT("%s: %s"),
		*Super::GetDescriptionTitle().ToString(), *Desc));
}

FText UEnvQueryTest_ParallelToCover::GetDescriptionDetails() const
{
	return DescribeFloatTestParams();
}

#undef LOCTEXT_NAMESPACE