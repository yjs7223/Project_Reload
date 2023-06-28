//// Copyright, (c) Sami Kangasmaa 2022


#include "AI/Tests/EnvQueryTest_FreeCover.h"
#include "AI/EnvQueryItemType_CoverBase.h"
#include "CoverSystem.h"
#include "AIController.h"

#define LOCTEXT_NAMESPACE "EnvQueryGenerator"

UEnvQueryTest_FreeCover::UEnvQueryTest_FreeCover()
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_CoverBase::StaticClass();
	SetWorkOnFloatValues(false);
}

void UEnvQueryTest_FreeCover::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	AController* AI = nullptr;
	if (APawn* Pawn = Cast<APawn>(QueryOwner))
	{
		AI = Pawn->GetController();
	}

	BoolValue.BindData(QueryOwner, QueryInstance.QueryID);
	const bool bRequireFree = BoolValue.GetValue();

	ACoverSystem* CoverSystem = ACoverSystem::GetCoverSystem(QueryInstance.World);
	if (CoverSystem)
	{
		for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
		{
			const FCover CoverItem = UEnvQueryItemType_CoverBase::GetValue(QueryInstance.RawData.GetData() + QueryInstance.Items[It.GetIndex()].DataOffset);
			AController* OccupyingController = CoverSystem->GetOccupyingController(CoverItem.Handle);
			bool bIsFree = OccupyingController == nullptr;

			if (AI)
			{
				bool bSelfOccupy = (OccupyingController == AI);
				if (!bIsFree && bSelfOccupy)
				{
					bIsFree = true;
				}
			}

			It.SetScore(TestPurpose, FilterType, bIsFree, bRequireFree);
		}
	}
}

FText UEnvQueryTest_FreeCover::GetDescriptionDetails() const
{
	return FText::Format(FText::FromString("{0}"), DescribeBoolTestParams("free"));
}

#undef LOCTEXT_NAMESPACE