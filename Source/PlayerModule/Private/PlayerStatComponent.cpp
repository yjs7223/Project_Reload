// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatComponent.h"
#include "BaseCharacter.h"
#include "MatineeCameraShake.h"
#include "InteractiveComponent.h"
#include "PlayerWeaponComponent.h"
#include "PlayerInputComponent.h"
//#include "GameFramework/CharacterMovementComponent.h"


UPlayerStatComponent::UPlayerStatComponent()
{
	TargetEnemy = nullptr;
}

void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	diePlay.__Internal_AddDynamic(this, &UPlayerStatComponent::Revive, FName("Revive"));

	revivePos = GetOwner()->GetActorLocation();
	revivePos.Z += 50.0f;
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
	//SetComponentTickInterval(0.2f);

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
	if (curHP >= maxHP)
	{
		GetWorld()->GetTimerManager().ClearTimer(StandbyTimer);
		GetWorld()->GetTimerManager().ClearTimer(HpRecoverTimer);
	}
	OnChangedHealthDelegate.Broadcast(curHP / maxHP);
	OnVisibleAttackedUIDelegate.ExecuteIfBound();
}

void UPlayerStatComponent::Attacked(float p_damage, ABaseCharacter* attacker, EHitType hittype, FVector attackPoint)
{
	Super::Attacked(p_damage, attacker, hittype, attackPoint);
	GetWorld()->GetTimerManager().ClearTimer(StandbyTimer);
	GetWorld()->GetTimerManager().ClearTimer(HpRecoverTimer);

	TargetEnemy = attacker;

	if (AttackedCameraShake)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(AttackedCameraShake, 1.0f);

	}

	if (!bDie)
	{
		owner->FindComponentByClass<UPlayerWeaponComponent>()->OnCombatWidgetVisible.Broadcast(true);
	}
	//OnVisibleHPUIDelegate.Broadcast();
	OnChangedHealthDelegate.Broadcast(curHP / maxHP);
	OnVisibleAttackedUIDelegate.ExecuteIfBound();
	if (attacker)
	{
		OnCreateAttackedUIDelegate.Broadcast(attacker);
	}

	GetWorld()->GetTimerManager().SetTimer(StandbyTimer, FTimerDelegate::CreateLambda([&]()
		{
			GetWorld()->GetTimerManager().SetTimer(HpRecoverTimer, FTimerDelegate::CreateLambda([&]()
				{
					RecoverHP(5.0f);
				}), 0.1f, true);

		}), 10.f, false);
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
		OnSetInteractUIDelegate.Broadcast(result.ImpactPoint);

		//상호작용 오브젝트가 존재하지않거나 다른오브젝트일경우 타겟변경
		if (!InteractActor || InteractActor != result.GetActor())
		{
			InteractActor = result.GetActor();
		}
		UInteractiveComponent* myinteractive = InteractActor->FindComponentByClass<UInteractiveComponent>();

		//상호작용 컴폰넌트가 존재하고 상호작용이 되지않은경우에만 활성화
		if (myinteractive && !myinteractive->bInteractive && !myinteractive->bActive)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("intercheck"));
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
					InteractActor = nullptr;
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

void UPlayerStatComponent::Revive()
{
	FTimerHandle dieTimer;
	UPlayerInputComponent* inputComp = GetOwner()->FindComponentByClass<UPlayerInputComponent>();
	if (inputComp)
	{
		inputComp->SetInputEnable(false);
		inputComp->StopFire();
		inputComp->StopAiming();
		inputComp->StopReload();
		inputComp->StopFire();
		inputComp->getInput()->movevec = FVector::ZeroVector;
	}

	GetWorld()->GetTimerManager().SetTimer(dieTimer, FTimerDelegate::CreateLambda([=]()
		{
			bDie = false;
			RecoverHP(10000);
			UPlayerInputComponent* inputComp = GetOwner()->FindComponentByClass<UPlayerInputComponent>();
			if (inputComp)
			{
				inputComp->SetInputEnable(true);
				
			}
			
			GetOwner()->SetActorLocation(revivePos);

		}), 2.f, false);
}


