// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_SupportState.h"
#include "BaseAICtr.h"
#include "AICommander.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_SupportState::UBTS_SupportState()
{
	Dis_start = false;
}

void UBTS_SupportState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	
	for (auto sup : Cast<AAICommander>(commander)->List_Suppression)
	{
		if (sup.Value >= 90.0f)
		{
			Sup_Vec = *Cast<AAICommander>(commander)->List_Location.Find(sup.Key);
			for (auto Loc : Cast<AAICommander>(commander)->List_Location)
			{
				if (Loc.Key != *Cast<AAICommander>(commander)->List_Location.FindKey(Sup_Vec))
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
			AIController = nullptr;
			ACharacter = Cast<AAICharacter>(*Cast<AAICommander>(commander)->List_Division.FindKey(Min_Dis_Key));
			if (ACharacter)
			{
				AIController = Cast<ABaseAICtr>(Cast<AAICharacter>((*Cast<AAICommander>(commander)->List_Division.FindKey(Min_Dis_Key)))->GetController());
			}
			if (AIController)
			{
				if (AIController->BlackboardComponent)
				{
					BlackboardComponent = AIController->BlackboardComponent;
					BlackboardComponent->SetValueAsBool("AI_Support", true);
				}
			}
			Dis_start = false;
		}
		else
		{
			for (auto com : Cast<AAICommander>(commander)->List_Combat)
			{
				if (com.Value == ECombat::Move)
				{
					Com_Vec = *Cast<AAICommander>(commander)->List_Location.Find(com.Key);
					for (auto Loc : Cast<AAICommander>(commander)->List_Location)
					{
						if (Loc.Key != *Cast<AAICommander>(commander)->List_Location.FindKey(Com_Vec))
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
			AIController = nullptr;
			ACharacter = Cast<AAICharacter>(*Cast<AAICommander>(commander)->List_Division.FindKey(Min_Dis_Key));
			if (ACharacter)
			{
				AIController = Cast<ABaseAICtr>(Cast<AAICharacter>((*Cast<AAICommander>(commander)->List_Division.FindKey(Min_Dis_Key)))->GetController());
			}
			if (AIController)
			{
				if (AIController->BlackboardComponent)
				{
					BlackboardComponent = AIController->BlackboardComponent;
					BlackboardComponent->SetValueAsBool("AI_Support", true);
				}
			}
			Dis_start = false;
		}
	}

}
