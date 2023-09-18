// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncNode.h"
#include "AIController.h"	
#include "Kismet/GameplayStatics.h"

UAsyncNode* UAsyncNode::DroneMoveTo(const int ID,AAIController* ctr, FVector loc, float p_AccRadius)
{
	UAsyncNode* NewFeroxNod = NewObject<UAsyncNode>();
	NewFeroxNod->temp = ID;
	NewFeroxNod->m_ctr = ctr;
	NewFeroxNod->m_MoveLoc = loc;
	NewFeroxNod->m_AcceptanceRadius = p_AccRadius;

	// OnMoveCompleted �̺�Ʈ �ڵ鷯�� ����
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


	/*AActor* pc = Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	m_ctr->MoveToActor(pc);*/
	//isEqual();
}

//void UAsyncNode::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
//{
//	if (Result.IsSuccess())
//	{
//		// �̵��� ���������� �Ϸ��
//		OnSuccess.Broadcast(temp, "Success! from c++");
//	}
//	else
//	{
//		// �̵��� ������
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