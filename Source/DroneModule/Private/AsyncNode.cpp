// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncNode.h"
#include "AIController.h"	

UAsyncNode* UAsyncNode::DroneMoveTo(const int ID,AAIController* ctr, FVector loc, float p_AccRadius)
{
	UAsyncNode* NewFeroxNod = NewObject<UAsyncNode>();
	NewFeroxNod->temp = ID;
	NewFeroxNod->m_ctr = ctr;
	NewFeroxNod->m_MoveLoc = loc;
	NewFeroxNod->m_AcceptanceRadius = p_AccRadius;

	// OnMoveCompleted 이벤트 핸들러를 설정
	/*if (ctr)
	{
		ctr->ReceiveMoveCompleted.AddDynamic(NewFeroxNod, &UAsyncNode::OnMoveCompleted);
	}*/
	//OnSuccess =
	//	m_ctr->ReceiveMoveCompleted.Broadcast(1, EPathFollowingResult::Success);
	return NewFeroxNod;
}

void UAsyncNode::Activate()
{
	m_ctr->MoveToLocation(m_MoveLoc, m_AcceptanceRadius, false, false);

	//isEqual();
}

//void UAsyncNode::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
//{
//	if (Result.IsSuccess())
//	{
//		// 이동이 성공적으로 완료됨
//		OnSuccess.Broadcast(temp, "Success! from c++");
//	}
//	else
//	{
//		// 이동이 실패함
//		OnFail.Broadcast(temp, "Fail");
//	}
//	RemoveFromRoot();
//}
//



void UAsyncNode::isEqual()
{
	m_ctr->MoveToLocation(m_MoveLoc, m_AcceptanceRadius, false, false);

	float dis = FVector::Distance(m_ctr->GetPawn()->GetActorLocation(), m_MoveLoc);

	if (dis < 100)
	{
		OnSuccess.Broadcast(temp, "Success! from c++");
	}
	/*else
	{
		OnFail.Broadcast(-228, "Fail");
	}*/
	RemoveFromRoot();
}