// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_SupportState.h"
#include "AI_Controller.h"
#include "AICommander.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_SupportState::UBTS_SupportState()
{
	NodeName = TEXT("SupportState");
	Dis_start = false;
}

void UBTS_SupportState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World) return;
	
	for (auto sup : Cast<AAI_Controller>(OwnerComp.GetAIOwner())->commander->List_Suppression)
	{
		if (sup.Value >= 90.0f)
		{
			Sup_Vec = *Cast<AAI_Controller>(OwnerComp.GetAIOwner())->commander->List_Location.Find(sup.Key);
			for (auto Loc : Cast<AAI_Controller>(OwnerComp.GetAIOwner())->commander->List_Location)
			{
				if (Loc.Key != *Cast<AAI_Controller>(OwnerComp.GetAIOwner())->commander->List_Location.FindKey(Sup_Vec))
				{
					if (!Dis_start)
					{
						Min_Dis = FVector::Distance(Sup_Vec, Loc.Value);
						Min_Dis_Key = Loc.Key;
						Dis_start = true;
					}
					else
					{
						Dis = FVector::Distance(Sup_Vec, Loc.Value);
						if (Dis <= Min_Dis)
						{
							Min_Dis = Dis;
							Min_Dis_Key = Loc.Key;
						}
					}
				}
				
			}
			Cast<AAI_Controller>(OwnerComp.GetAIOwner())->commander->BlackboardComponent->SetValueAsBool("AI_Support", true);
			Dis_start = false;
		}
		else
		{
			for (auto com : Cast<AAI_Controller>(OwnerComp.GetAIOwner())->commander->List_Combat)
			{
				if (com.Value == ECombat::Move)
				{
					Com_Vec = *Cast<AAI_Controller>(OwnerComp.GetAIOwner())->commander->List_Location.Find(com.Key);
					for (auto Loc : Cast<AAI_Controller>(OwnerComp.GetAIOwner())->commander->List_Location)
					{
						if (Loc.Key != *Cast<AAI_Controller>(OwnerComp.GetAIOwner())->commander->List_Location.FindKey(Com_Vec))
						{
							if (!Dis_start)
							{
								Min_Dis = FVector::Distance(Com_Vec, Loc.Value);
								Min_Dis_Key = Loc.Key;
								Dis_start = true;
							}
							else
							{
								Dis = FVector::Distance(Com_Vec, Loc.Value);
								if (Dis <= Min_Dis)
								{
									Min_Dis = Dis;
									Min_Dis_Key = Loc.Key;
								}
							}
						}
					}
				}
			}
			Cast<AAI_Controller>(OwnerComp.GetAIOwner())->commander->BlackboardComponent->SetValueAsBool("AI_Support", true);			
			Dis_start = false;
		}
	}

}
