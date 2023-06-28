//// Copyright, (c) Sami Kangasmaa 2022


#include "AI/Tasks/BTTask_EQS_Query_Cover.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "VisualLogger/VisualLogger.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AISystem.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "AI/BlackboardKeyType_Cover.h"
#include "AI/EnvQueryItemType_CoverBase.h"

namespace EQS_Query_CoverNS
{
	void AddCoverFilter(FBlackboardKeySelector& BlackboardKey, UObject* Owner, FName PropertyName)
	{
		const FString FilterName = PropertyName.ToString() + TEXT("_Cover");
		BlackboardKey.AllowedTypes.Add(NewObject<UBlackboardKeyType_Cover>(Owner, *FilterName));
	}
}

UBTTask_EQS_Query_Cover::UBTTask_EQS_Query_Cover(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Cover EQS Query";

	// filter with keys that have matching env query values, only for instances
	// CDO won't be able to access types from game dlls
	if (GIsEditor && !HasAnyFlags(RF_ClassDefaultObject))
	{
		CollectKeyFilters();
	}

	QueryFinishedDelegate = FQueryFinishedSignature::CreateUObject(this, &UBTTask_EQS_Query_Cover::OnQueryFinished);

#if COVER_ENGINE_VERSION >= 500
	bUpdateBBOnFail = GET_AI_CONFIG_VAR(bClearBBEntryOnBTEQSFail);
#else
	bUpdateBBOnFail = true;
#endif

	LocationBlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_EQS_Query_Cover, LocationBlackboardKey));
	EQS_Query_CoverNS::AddCoverFilter(BlackboardKey, this, GET_MEMBER_NAME_CHECKED(UBTTask_EQS_Query_Cover, BlackboardKey));
}

void UBTTask_EQS_Query_Cover::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	EQSRequest.InitForOwnerAndBlackboard(*this, GetBlackboardAsset());

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		LocationBlackboardKey.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize task: %s, make sure that behavior tree specifies blackboard asset!"), *GetName());
	}
}

EBTNodeResult::Type UBTTask_EQS_Query_Cover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* QueryOwner = OwnerComp.GetOwner();
	AController* ControllerOwner = Cast<AController>(QueryOwner);
	if (ControllerOwner)
	{
		QueryOwner = ControllerOwner->GetPawn();
	}

	if (QueryOwner && EQSRequest.IsValid())
	{
		const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
		FBTEnvQueryTaskMemory* MyMemory = CastInstanceNodeMemory<FBTEnvQueryTaskMemory>(NodeMemory);

		MyMemory->RequestID = EQSRequest.Execute(*QueryOwner, BlackboardComponent, QueryFinishedDelegate);

		const bool bValid = (MyMemory->RequestID >= 0);
		if (bValid)
		{
			WaitForMessage(OwnerComp, UBrainComponent::AIMessage_QueryFinished, MyMemory->RequestID);
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_EQS_Query_Cover::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UWorld* MyWorld = OwnerComp.GetWorld();
	UEnvQueryManager* QueryManager = UEnvQueryManager::GetCurrent(MyWorld);

	if (QueryManager)
	{
		FBTEnvQueryTaskMemory* MyMemory = (FBTEnvQueryTaskMemory*)NodeMemory;
		QueryManager->AbortQuery(MyMemory->RequestID);
	}

	return EBTNodeResult::Aborted;
}

FString UBTTask_EQS_Query_Cover::GetStaticDescription() const
{
	return EQSRequest.bUseBBKeyForQueryTemplate ? FString::Printf(TEXT("%s: EQS query indicated by %s blackboard key\nResult Blackboard key: %s"), *Super::GetStaticDescription(), *EQSRequest.EQSQueryBlackboardKey.SelectedKeyName.ToString(), *BlackboardKey.SelectedKeyName.ToString())
		: FString::Printf(TEXT("%s: %s\nResult Blackboard key: %s"), *Super::GetStaticDescription(), *GetNameSafe(EQSRequest.QueryTemplate), *BlackboardKey.SelectedKeyName.ToString());
}

void UBTTask_EQS_Query_Cover::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);

	if (Verbosity == EBTDescriptionVerbosity::Detailed)
	{
		FBTEnvQueryTaskMemory* MyMemory = (FBTEnvQueryTaskMemory*)NodeMemory;
		Values.Add(FString::Printf(TEXT("request: %d"), MyMemory->RequestID));
	}
}

uint16 UBTTask_EQS_Query_Cover::GetInstanceMemorySize() const
{
	return sizeof(FBTEnvQueryTaskMemory);
}

void UBTTask_EQS_Query_Cover::OnQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	if (Result->IsAborted())
	{
		return;
	}

	AActor* MyOwner = Cast<AActor>(Result->Owner.Get());
	if (APawn* PawnOwner = Cast<APawn>(MyOwner))
	{
		MyOwner = PawnOwner->GetController();
	}

	UBehaviorTreeComponent* MyComp = MyOwner ? MyOwner->FindComponentByClass<UBehaviorTreeComponent>() : NULL;
	if (!MyComp)
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Unable to find behavior tree to notify about finished query from %s!"), *GetNameSafe(MyOwner));
		return;
	}

#if COVER_ENGINE_VERSION >= 500
	bool bSuccess = Result->IsSuccessful() && (Result->Items.Num() >= 1);
#else
	bool bSuccess = Result->IsSuccsessful() && (Result->Items.Num() >= 1);
#endif

	if (bSuccess)
	{
		UBlackboardComponent* MyBlackboard = MyComp->GetBlackboardComponent();
		UEnvQueryItemType* ItemTypeCDO = Result->ItemType->GetDefaultObject<UEnvQueryItemType>();

		bSuccess = ItemTypeCDO->StoreInBlackboard(BlackboardKey, MyBlackboard, Result->RawData.GetData() + Result->Items[0].DataOffset);
		if (!bSuccess)
		{
			UE_VLOG(MyOwner, LogBehaviorTree, Warning, TEXT("Failed to store query result! item:%s key:%s"),
				*UEnvQueryTypes::GetShortTypeName(Result->ItemType).ToString(),
				*UBehaviorTreeTypes::GetShortTypeName(BlackboardKey.SelectedKeyType));
		}

		UEnvQueryItemType_CoverBase* CoverItemTypeCDO = Cast<UEnvQueryItemType_CoverBase>(ItemTypeCDO);
		bSuccess = CoverItemTypeCDO != nullptr;
		if (bSuccess)
		{
			check(CoverItemTypeCDO);
			const FVector ItemLocation = CoverItemTypeCDO->GetItemLocation(Result->RawData.GetData() + Result->Items[0].DataOffset);
			bSuccess = MyBlackboard->SetValue<UBlackboardKeyType_Vector>(LocationBlackboardKey.GetSelectedKeyID(), ItemLocation);
		}

		if (!bSuccess)
		{
			UE_VLOG(MyOwner, LogBehaviorTree, Warning, TEXT("Failed to store query result! item:%s key:%s"),
				*UEnvQueryTypes::GetShortTypeName(Result->ItemType).ToString(),
				*UBehaviorTreeTypes::GetShortTypeName(BlackboardKey.SelectedKeyType));
		}
	}
	else if (bUpdateBBOnFail)
	{
		UBlackboardComponent* MyBlackboard = MyComp->GetBlackboardComponent();
		check(MyBlackboard);
		MyBlackboard->ClearValue(BlackboardKey.GetSelectedKeyID());
		MyBlackboard->ClearValue(LocationBlackboardKey.GetSelectedKeyID());
	}

	FAIMessage::Send(MyComp, FAIMessage(UBrainComponent::AIMessage_QueryFinished, this, Result->QueryID, bSuccess));
}

void UBTTask_EQS_Query_Cover::CollectKeyFilters() {}

void UBTTask_EQS_Query_Cover::PostLoad()
{
	Super::PostLoad();
}

#if WITH_EDITOR
void UBTTask_EQS_Query_Cover::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty &&
		PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UBTTask_RunEQSQuery, EQSRequest))
	{
		EQSRequest.PostEditChangeProperty(*this, PropertyChangedEvent);
	}
}

FName UBTTask_EQS_Query_Cover::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Task.RunEQSQuery.Icon");
}

#endif
