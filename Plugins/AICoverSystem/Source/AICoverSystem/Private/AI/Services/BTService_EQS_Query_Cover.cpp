// Copyright (c), Sami Kangasmaa 2022


#include "AI/Services/BTService_EQS_Query_Cover.h"

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

namespace EQS_Query_Service_CoverNS
{
	void AddCoverFilter(FBlackboardKeySelector& BlackboardKey, UObject* Owner, FName PropertyName)
	{
		const FString FilterName = PropertyName.ToString() + TEXT("_Cover");
		BlackboardKey.AllowedTypes.Add(NewObject<UBlackboardKeyType_Cover>(Owner, *FilterName));
	}
}

UBTService_EQS_Query_Cover::UBTService_EQS_Query_Cover()
{
	NodeName = "Cover EQS query on a regular basis";

	bNotifyBecomeRelevant = false;
	bNotifyCeaseRelevant = true;

	LocationBlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_EQS_Query_Cover, LocationBlackboardKey));
	EQS_Query_Service_CoverNS::AddCoverFilter(BlackboardKey, this, GET_MEMBER_NAME_CHECKED(UBTService_EQS_Query_Cover, BlackboardKey));

	QueryFinishedDelegate = FQueryFinishedSignature::CreateUObject(this, &UBTService_EQS_Query_Cover::OnQueryFinished);

#if COVER_ENGINE_VERSION >= 500
	bUpdateBBOnFail = GET_AI_CONFIG_VAR(bClearBBEntryOnBTEQSFail);
#else
	bUpdateBBOnFail = true;
#endif
}

void UBTService_EQS_Query_Cover::InitializeFromAsset(UBehaviorTree& Asset)
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

void UBTService_EQS_Query_Cover::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
		FBTCoverEQSServiceMemory* MyMemory = CastInstanceNodeMemory<FBTCoverEQSServiceMemory>(NodeMemory);

		// Trigger new query only if the previous one has already finished
		if (MyMemory->RequestID == INDEX_NONE)
		{
			MyMemory->RequestID = EQSRequest.Execute(*QueryOwner, BlackboardComponent, QueryFinishedDelegate);
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}

FString UBTService_EQS_Query_Cover::GetStaticDescription() const
{
	return EQSRequest.bUseBBKeyForQueryTemplate ? FString::Printf(TEXT("%s: EQS query indicated by %s blackboard key\nResult Blackboard key: %s"), *Super::GetStaticDescription(), *EQSRequest.EQSQueryBlackboardKey.SelectedKeyName.ToString(), *BlackboardKey.SelectedKeyName.ToString())
		: FString::Printf(TEXT("%s: %s\nResult Blackboard key: %s"), *Super::GetStaticDescription(), *GetNameSafe(EQSRequest.QueryTemplate), *BlackboardKey.SelectedKeyName.ToString());
}

void UBTService_EQS_Query_Cover::OnQueryFinished(TSharedPtr<FEnvQueryResult> Result)
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

	UBehaviorTreeComponent* BTComp = MyOwner ? MyOwner->FindComponentByClass<UBehaviorTreeComponent>() : NULL;
	if (!BTComp)
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Unable to find behavior tree to notify about finished query from %s!"), *GetNameSafe(MyOwner));
		return;
	}

	FBTCoverEQSServiceMemory* MyMemory = CastInstanceNodeMemory<FBTCoverEQSServiceMemory>(BTComp->GetNodeMemory(this, BTComp->FindInstanceContainingNode(this)));
	check(MyMemory);
	ensure(MyMemory->RequestID != INDEX_NONE);

#if COVER_ENGINE_VERSION >= 500
	bool bSuccess = Result->IsSuccessful() && (Result->Items.Num() >= 1);
#else
	bool bSuccess = Result->IsSuccsessful() && (Result->Items.Num() >= 1);
#endif

	if (bSuccess)
	{
		UBlackboardComponent* MyBlackboard = BTComp->GetBlackboardComponent();
		UEnvQueryItemType* ItemTypeCDO = Result->ItemType->GetDefaultObject<UEnvQueryItemType>();

		bSuccess = ItemTypeCDO->StoreInBlackboard(BlackboardKey, MyBlackboard, Result->RawData.GetData() + Result->Items[0].DataOffset);

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
		UBlackboardComponent* MyBlackboard = BTComp->GetBlackboardComponent();
		check(MyBlackboard);
		MyBlackboard->ClearValue(BlackboardKey.GetSelectedKeyID());
		MyBlackboard->ClearValue(LocationBlackboardKey.GetSelectedKeyID());
	}

	MyMemory->RequestID = INDEX_NONE;
}

void UBTService_EQS_Query_Cover::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTCoverEQSServiceMemory* MyMemory = CastInstanceNodeMemory<FBTCoverEQSServiceMemory>(NodeMemory);
	check(MyMemory);

	if (MyMemory->RequestID != INDEX_NONE)
	{
		UWorld* World = OwnerComp.GetWorld();
		if (World)
		{
			// make EQS abort that query
			UEnvQueryManager* EQSManager = UEnvQueryManager::GetCurrent(World);
			check(EQSManager);
			EQSManager->AbortQuery(MyMemory->RequestID);
		}

		MyMemory->RequestID = INDEX_NONE;
	}

	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

void UBTService_EQS_Query_Cover::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	FBTCoverEQSServiceMemory* MyMemory = CastInstanceNodeMemory<FBTCoverEQSServiceMemory>(NodeMemory);
	check(MyMemory);
	MyMemory->RequestID = INDEX_NONE;
}

#if WITH_EDITOR

void UBTService_EQS_Query_Cover::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const
{
	const FBTCoverEQSServiceMemory* MyMemory = CastInstanceNodeMemory<FBTCoverEQSServiceMemory>(NodeMemory);
	check(MyMemory);
	ensure(MyMemory->RequestID == INDEX_NONE);
}

void UBTService_EQS_Query_Cover::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty &&
		PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UBTService_EQS_Query_Cover, EQSRequest))
	{
		EQSRequest.PostEditChangeProperty(*this, PropertyChangedEvent);
	}
}

#endif // WITH_EDITOR
