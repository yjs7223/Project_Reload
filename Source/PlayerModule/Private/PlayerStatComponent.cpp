// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatComponent.h"
#include "BaseCharacter.h"
#include "MatineeCameraShake.h"
#include "InteractiveComponent.h"


UPlayerStatComponent::UPlayerStatComponent()
{
	TargetEnemy = nullptr;
}

void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerStatComponent::BeginDestroy()
{
	OnVisibleHPUIDelegate.Clear();
	OnChangedHealthDelegate.Clear();
	OnVisibleAttackedUIDelegate.Unbind();
	OnCreateAttackedUIDelegate.Clear();
	OnVisibleInteractiveUIDelegate.Unbind();


	Super::BeginDestroy();
}

void UPlayerStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SetComponentTickInterval(0.2f);

	CheckInteractiveObj();
}

void UPlayerStatComponent::SetHP(float p_HP)
{
	Super::SetHP(p_HP);

	OnChangedHealthDelegate.Broadcast(curHP / maxHP);
	//OnChangedHealthDelegate.ExecuteIfBound(curHP / maxHP);
}

void UPlayerStatComponent::RecoverHP(float p_HP)
{
	Super::RecoverHP(p_HP);

	OnChangedHealthDelegate.Broadcast(curHP / maxHP);
}

void UPlayerStatComponent::Attacked(float p_damage, ABaseCharacter* attacker, EHitType hittype, FVector attackPoint)
{
	Super::Attacked(p_damage, attacker, hittype, attackPoint);

	TargetEnemy = attacker;

	if (AttackedCameraShake)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(AttackedCameraShake, 1.0f);

	}

	OnVisibleHPUIDelegate.Broadcast();
	OnChangedHealthDelegate.Broadcast(curHP / maxHP);
	OnVisibleAttackedUIDelegate.ExecuteIfBound();
	OnCreateAttackedUIDelegate.Broadcast(attacker);
}

void UPlayerStatComponent::CheckInteractiveObj()
{
	FVector start;
	FRotator cameraRotation;
	owner->Controller->GetPlayerViewPoint(start, cameraRotation);
	FVector end = start + (cameraRotation.Vector() * 500.0f);
	FHitResult result;

	if (GetWorld()->SweepSingleByChannel(result, start, end, FQuat::Identity, ECC_GameTraceChannel8, FCollisionShape::MakeSphere(30.0f)))
	{
		//DrawDebugSphere(GetWorld(), result.Location, 30.0f, 50.0f, FColor::Red, true);
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("intercheck"));

		//상호작용 오브젝트가 존재하지않거나 다른오브젝트일경우 타겟변경
		if (!InteractActor || InteractActor != result.GetActor())
		{
			InteractActor = result.GetActor();
		}
		UInteractiveComponent* myinteractive = InteractActor->FindComponentByClass<UInteractiveComponent>();

		//상호작용 컴폰넌트가 존재하고 상호작용이 되지않은경우에만 활성화
		if (myinteractive && !myinteractive->bInteractive && !myinteractive->bActive)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("intercheck"));
			myinteractive->ActiveInteractable();
			OnVisibleInteractiveUIDelegate.ExecuteIfBound(myinteractive->bActive, InteractActor);
		}
	}
	else
	{
		if (InteractActor)
		{
			UInteractiveComponent* myinteractive = InteractActor->FindComponentByClass<UInteractiveComponent>();
			if (myinteractive && myinteractive->bActive)
			{
				if (!myinteractive->bInteractive)
				{
					myinteractive->ActiveInteractable();
					OnVisibleInteractiveUIDelegate.ExecuteIfBound(myinteractive->bActive, InteractActor);
				}
			}
		}
	}
}

void UPlayerStatComponent::Interacting()
{
	if (InteractActor)
	{
		if (UInteractiveComponent* intercomp = InteractActor->FindComponentByClass<UInteractiveComponent>())
		{
			if (!intercomp->bInteractive)
			{
				intercomp->StartInteract();
				OnVisibleInteractiveUIDelegate.ExecuteIfBound(intercomp->bActive, InteractActor);
			}
		}
	}
}

