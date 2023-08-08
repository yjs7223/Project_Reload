// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_DetourCoverSelection.h"
#include "AI_Controller.h"
#include "AICommander.h"
#include "AICharacter.h"
#include "CoverComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/World.h"
#include "SubEncounterSpace.h"

UBTT_DetourCoverSelection::UBTT_DetourCoverSelection()
{
	NodeName = TEXT("DetourCoverSelection");
	B_distance = false;
	Detourchange = false;
	B_distance = false;
	select_ai = nullptr;
}

EBTNodeResult::Type UBTT_DetourCoverSelection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	if (!commander)
	{
		commander = Cast<AAICommander>(OwnerComp.GetAIOwner());
	}
	if (!player)
	{
		player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}
	if (player && player->FindComponentByClass<UCoverComponent>()->GetCoverWall())
	{
		commander->DetourCoverPoint();
		if (!commander->DetourCoverArray.IsEmpty())
		{
			for (auto ai : commander->List_Division)
			{
				Cast<AAICharacter>(ai.Key)->Detour = false;
			}
			for (auto cover : commander->DetourCoverArray)
			{
				select_ai = nullptr;
				Detourchange = false;
				B_distance = false;
				for (auto coverlist : commander->List_CoverPoint)
				{
					if (FVector::Distance(cover, coverlist.Value) < 200)
					{
						B_distance = true;
					}
				}
				for (auto subAi : commander->Now_suben->AIArray)
				{
					if (!Cast<AAI_Controller>(Cast<AAICharacter>(subAi)->GetController()))
					{
						continue;
					}
					AAI_Controller* sub_aic = Cast<AAI_Controller>(Cast<AAICharacter>(subAi)->GetController());
					if (FVector::Distance(cover, sub_aic->GetBlackboardComponent()->GetValueAsVector("AI_MoveLocation")) < 200)
					{
						B_distance = true;
					}
				}
				if (!B_distance)
				{
					for (auto ai : commander->List_Division)
					{
						if (!Cast<AAICharacter>(ai.Key)->Detour)
						{
							Detourchange = true;
							FVector Find_rot = UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), cover).Vector();
							Find_rot.Normalize();
							float Dot_Cover = FVector::DotProduct(player->GetCapsuleComponent()->GetForwardVector(), Find_rot);
							float angle = FMath::RadiansToDegrees(FMath::Acos(Dot_Cover));
							if (angle < commander->detour_angle && angle > commander->ndetour_angle)
							{
								if (*commander->List_Suppression.Find(ai.Value) < 30.0f)
								{
									AIController = nullptr;
									AIController = Cast<AAI_Controller>(Cast<AAICharacter>(ai.Key)->GetController());
									if (AIController)
									{
										if (AIController->GetBlackboardComponent())
										{
											
											if (AIController->GetBlackboardComponent()->GetValueAsEnum("Combat") == (uint8)ECombat::InCover)
											{
												
												if (select_ai == nullptr)
												{
													Dis_Loc = FVector::Distance(cover, ai.Key->GetActorLocation());
													select_ai = ai.Key;
												}
												else
												{
													if (Dis_Loc > FVector::Distance(cover, ai.Key->GetActorLocation()))
													{
														Dis_Loc = FVector::Distance(cover, ai.Key->GetActorLocation());
														select_ai = ai.Key;
													}
												}
											}
												
										}
										
									}
								}
							}
							
						}
					}
				}
				if (!Detourchange)
				{
					return EBTNodeResult::Succeeded;
				}
				if (select_ai)
				{
					
					AIController = nullptr;
					AIController = Cast<AAI_Controller>(Cast<AAICharacter>(select_ai)->GetController());
					if (AIController)
					{
						if (AIController->GetBlackboardComponent())
						{
							FActorSpawnParameters spawnParams;
							FRotator rotator;
							FVector RedballLoc = cover + FVector(0, 0, 100);
							AIController->GetBlackboardComponent()->SetValueAsVector("AI_CoverLocation", cover);
							GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "cover.ToString()");
							GetWorld()->SpawnActor<AActor>(commander->RedBallBlueprint, cover, rotator);
							Cast<AAICharacter>(select_ai)->Detour = true;
							commander->List_CoverPoint.Add(*commander->List_Division.Find(select_ai), cover);
						}
					}
				}
			}
		}
		
	}
	
	return EBTNodeResult::Succeeded;
}


