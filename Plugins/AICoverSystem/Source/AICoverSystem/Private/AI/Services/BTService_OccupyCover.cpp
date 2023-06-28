//// Copyright, (c) Sami Kangasmaa 2022


#include "AI/Services/BTService_OccupyCover.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/BlackboardKeyType_Cover.h"
#include "VisualLogger/VisualLogger.h"

namespace OccupyCoverNS 
{
	void AddCoverFilter(FBlackboardKeySelector& BlackboardKey, UObject* Owner, FName PropertyName)
	{
		const FString FilterName = PropertyName.ToString() + TEXT("_Cover");
		BlackboardKey.AllowedTypes.Add(NewObject<UBlackboardKeyType_Cover>(Owner, *FilterName));
	}
}

UBTService_OccupyCover::UBTService_OccupyCover(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Keep Cover Occupied";

	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;

	bTickIntervals = false;

	OccupyCoverNS::AddCoverFilter(BlackboardKey, this, GET_MEMBER_NAME_CHECKED(UBTService_OccupyCover, BlackboardKey));
}

FString UBTService_OccupyCover::GetStaticDescription() const
{
	FString KeyDesc("invalid");
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Cover::StaticClass())
	{
		KeyDesc = BlackboardKey.SelectedKeyName.ToString();
	}

	return FString::Printf(TEXT("Occupy cover from BB key %s"), *KeyDesc);
}

void UBTService_OccupyCover::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);

	FOccupyCoverServiceMemory* MyMemory = CastInstanceNodeMemory<FOccupyCoverServiceMemory>(NodeMemory);
	MyMemory->Cover = FCoverHandle();
}

void UBTService_OccupyCover::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	FOccupyCoverServiceMemory* MyMemory = CastInstanceNodeMemory<FOccupyCoverServiceMemory>(NodeMemory);
	check(MyMemory);
	MyMemory->Reset();

	AAIController* OwnerController = OwnerComp.GetAIOwner();
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	ACoverSystem* CoverSystem = ACoverSystem::GetCoverSystem(OwnerController);

	if (OwnerController != nullptr && MyBlackboard != nullptr && CoverSystem != nullptr)
	{
		if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Cover::StaticClass())
		{
			FCover Cover = MyBlackboard->GetValue<UBlackboardKeyType_Cover>(BlackboardKey.GetSelectedKeyID());
			if (Cover.IsValid())
			{
				if (CoverSystem->OccupyCover(OwnerController, Cover.Handle))
				{
					MyMemory->Cover = Cover.Handle;
				}
			}

			// Start observing blackboard value
			if (bObserveBlackboardValue)
			{
				if (MyMemory->BBObserverDelegateHandle.IsValid())
				{
					UE_VLOG(OwnerController, LogBehaviorTree, Warning, TEXT("UBTService_OccupyCover::ExecuteTask \'%s\' Old BBObserverDelegateHandle is still valid! Removing old Observer."), *GetNodeName());
					MyBlackboard->UnregisterObserver(BlackboardKey.GetSelectedKeyID(), MyMemory->BBObserverDelegateHandle);
				}
				MyMemory->BBObserverDelegateHandle = MyBlackboard->RegisterObserver(BlackboardKey.GetSelectedKeyID(), this, FOnBlackboardChangeNotification::CreateUObject(this, &UBTService_OccupyCover::OnBlackboardValueChange));
			}
		}
	}
}

void UBTService_OccupyCover::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	FOccupyCoverServiceMemory* MyMemory = CastInstanceNodeMemory<FOccupyCoverServiceMemory>(NodeMemory);
	check(MyMemory);

	AAIController* OwnerController = OwnerComp.GetAIOwner();
	ACoverSystem* CoverSystem = ACoverSystem::GetCoverSystem(OwnerController);

	if (OwnerController != nullptr && CoverSystem != nullptr)
	{
		if (MyMemory->Cover.IsValid())
		{
			CoverSystem->UnOccupyCoverFromController(MyMemory->Cover, OwnerController);
		}

		// Remove observer
		if (bObserveBlackboardValue)
		{
			UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
			if (ensure(BlackboardComp) && MyMemory->BBObserverDelegateHandle.IsValid())
			{
				BlackboardComp->UnregisterObserver(BlackboardKey.GetSelectedKeyID(), MyMemory->BBObserverDelegateHandle);
			}

			MyMemory->BBObserverDelegateHandle.Reset();
		}

		MyMemory->Reset();
	}
}

EBlackboardNotificationResult UBTService_OccupyCover::OnBlackboardValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	UBehaviorTreeComponent* BehaviorComp = Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent());
	if (BehaviorComp == nullptr)
	{
		return EBlackboardNotificationResult::RemoveObserver;
	}

	AAIController* MyController = BehaviorComp->GetAIOwner();
	uint8* RawMemory = BehaviorComp->GetNodeMemory(this, BehaviorComp->FindInstanceContainingNode(this));
	FOccupyCoverServiceMemory* MyMemory = CastInstanceNodeMemory<FOccupyCoverServiceMemory>(RawMemory);

	ACoverSystem* CoverSystem = ACoverSystem::GetCoverSystem(MyController);

	// Change the occupy state to a new cover
	if (MyMemory != nullptr && MyController != nullptr && CoverSystem != nullptr)
	{
		// Remove the old one
		if (MyMemory->Cover.IsValid())
		{
			CoverSystem->UnOccupyCoverFromController(MyMemory->Cover, MyController);
		}

		MyMemory->Reset();

		// Occupy the new
		if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Cover::StaticClass())
		{
			const FCover Cover = Blackboard.GetValue<UBlackboardKeyType_Cover>(BlackboardKey.GetSelectedKeyID());
			if (Cover.IsValid())
			{
				if (CoverSystem->OccupyCover(MyController, Cover.Handle))
				{
					MyMemory->Cover = Cover.Handle;
				}
			}
		}
	}

	return EBlackboardNotificationResult::ContinueObserving;
}
