//// Copyright, (c) Sami Kangasmaa 2022


#include "CoverSystemUtils.h"

FCover UCoverSystemUtils::GetBlackboardValueAsCover(UBTNode* NodeOwner, const FBlackboardKeySelector& Key)
{
	UBlackboardComponent* BlackboardComp = UBTFunctionLibrary::GetOwnersBlackboard(NodeOwner);
	return BlackboardComp ? BlackboardComp->GetValue<UBlackboardKeyType_Cover>(Key.SelectedKeyName) : FCover();
}

void UCoverSystemUtils::SetBlackboardValueAsCover(UBTNode* NodeOwner, const FBlackboardKeySelector& Key, FCover Value)
{
	if (UBlackboardComponent* BlackboardComp = UBTFunctionLibrary::GetOwnersBlackboard(NodeOwner))
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Cover>(Key.SelectedKeyName, Value);
	}
}

bool UCoverSystemUtils::IsCoverValid(const FCover& Cover)
{
	return Cover.IsValid();
}

bool UCoverSystemUtils::IsCoverHandleValid(const FCoverHandle& Handle)
{
	return Handle.IsValid();
}
