// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncNode.h"

UAsyncNode* UAsyncNode::AsyncFerox(const int ID, ADroneAICharacter* p_dron)
{
	UAsyncNode* NewFeroxNod = NewObject<UAsyncNode>();
	NewFeroxNod->temp = ID;
	NewFeroxNod->m_dron = p_dron;
	return NewFeroxNod;
}

void UAsyncNode::Activate()
{
	isEqual();
}

void UAsyncNode::isEqual()
{
	m_dron->StartMoveTo();
	
	if (m_dron->IsFinish)
	{
		OnSuccess.Broadcast(temp, "Success! from c++");
	}
	else
	{
		OnFail.Broadcast(-228, "Fail");
	}
	RemoveFromRoot();
}