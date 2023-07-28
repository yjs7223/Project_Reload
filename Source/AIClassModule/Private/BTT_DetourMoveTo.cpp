// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_DetourMoveTo.h"

UBTT_DetourMoveTo::UBTT_DetourMoveTo()
{
	NodeName = TEXT("DetourMoveTo");
}

EBTNodeResult::Type UBTT_DetourMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{



	return EBTNodeResult::Succeeded();
}
