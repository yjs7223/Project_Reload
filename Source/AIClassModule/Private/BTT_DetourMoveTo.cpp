// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_DetourMoveTo.h"
#include "AI_Controller.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>

UBTT_DetourMoveTo::UBTT_DetourMoveTo()
{
	NodeName = TEXT("DetourMoveTo");
	radius = 5;
}

EBTNodeResult::Type UBTT_DetourMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	if (DetourArr.IsEmpty())
		return EBTNodeResult::Succeeded;


	while (FVector::Distance(finalLoc, OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation()) <= radius)
	{
		auto controller = OwnerComp.GetAIOwner()->GetPawn()->GetController();
		FVector nextMove = FVector::ZeroVector;


		//float shortcut;
		/*for (FVector var in DetourArr)
		{
			if (nextMove == FVector::ZeroVector)
				nextMove = var;
			else
			{
				FVector::Distance(nextMove, var);

			}

		}*/









		UAIBlueprintHelperLibrary::SimpleMoveToLocation(controller, nextMove);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;

}
