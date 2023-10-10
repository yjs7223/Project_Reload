// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_SupportState.h"
#include "AI_Controller.h"
#include "AICommander.h"
#include "AICharacter.h"
#include "AI_Controller.h"
#include "ST_Suppression.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_SupportState::UBTS_SupportState()
{

	//Interval = 0.5f;

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
	if (!aic)
	{
		aic = Cast<AAI_Controller>(OwnerComp.GetAIOwner());
	}
	for (auto sup : aic->commander->List_Suppression)
	{
d		if (sup.Value >= support_default)
		{
			Sup_Vec = *aic->commander->List_Location.Find(sup.Key);
			Dis_start = false;
			for (auto Loc : aic->commander->List_Location)
			{
				if (Loc.Key != *aic->commander->List_Location.FindKey(Sup_Vec))
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
			aic->commander->GetBlackboardComponent()->SetValueAsBool("AI_Support", true);
		}
		else
		{
			for (auto com : aic->commander->List_Division)
			{
				aic = Cast<AAI_Controller>(Cast<AAICharacter>(com.Key)->GetController());
				if (aic)
				{
					if (aic->GetBlackboardComponent())
					{
						if (aic->GetBlackboardComponent()->GetValueAsEnum("Combat") == (uint8)ECombat::Move)
						{
							Com_Vec = *aic->commander->List_Location.Find(com.Value);
							Dis_start = false;
							for (auto Loc : aic->commander->List_Location)
							{
								if (Loc.Key != *aic->commander->List_Location.FindKey(Com_Vec))
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
				}
				
			}
			if (aic)
			{
				if (aic->commander->GetBlackboardComponent())
				{
					aic->commander->GetBlackboardComponent()->SetValueAsBool("AI_Support", true);
				}
			}
		}
	}
}

void UBTS_SupportState::SetDataTable(FName EnemyName)
{
	FST_Suppression* SuppressionData = DT_Suppression->FindRow<FST_Suppression>(EnemyName, FString(""));
	if (SuppressionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData Succeed!"));

		support_default = SuppressionData->Support_Default;
	}

}