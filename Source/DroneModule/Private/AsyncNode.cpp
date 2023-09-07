// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncNode.h"

UAsyncNode* UAsyncNode::AsyncFerox(const int ID)
{
	UAsyncNode* NewFeroxNod = NewObject<UAsyncNode>();
	NewFeroxNod->temp = ID;
	return NewFeroxNod;
}

void UAsyncNode::Activate()
{
	isEqual();
}

void UAsyncNode::isEqual()
{
	
	if (temp<1)
	{
		OnSuccess.Broadcast(temp, "Success! from c++");
	}
	else
	{
		OnFail.Broadcast(-228, "Fail");
	}
	RemoveFromRoot();
}