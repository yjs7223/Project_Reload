// Fill out your copyright notice in the Description page of Project Settings.


#include "AIWeaponComponent.h"
#include "AICharacter.h"
#include "NiagaraFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>

UAIWeaponComponent::UAIWeaponComponent()
	: recoilMax_Radius(5), recoilMin_Radius(1),
	recoil_Range(5)
{
}

void UAIWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner<AAICharacter>();
	// ...

}


// Called every frame
void UAIWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAIWeaponComponent::ShotAI(FVector loc, FRotator rot)
{
	//bUseControllerRotationYaw = true;

	float x = 0, y = 0;

	x = FMath::RandRange(-recoil_Radius, recoil_Radius);
	y = FMath::RandRange(-recoil_Radius, recoil_Radius);

	FVector start = shotPos;
	FVector end = start + ((rot + FRotator(x, y, 0)).Vector() * 5000);
	FVector end2 = (rot + FRotator(x, y, 0)).Vector() * recoil_Range;
	FCollisionQueryParams traceParams;
	GetWorld()->LineTraceSingleByChannel(m_result, start, end, ECC_Visibility, traceParams);

	//DrawDebugLine(GetWorld(), start, end, FColor::Orange, false, 0.1f);

	// 총구화염 이펙트 생성
	UGameplayStatics::SpawnEmitterAtLocation(this, shotFX, start, rot, true);

	// 발사체 나이아가라 이펙트 생성
	shotFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, shotFXNiagara, start, rot + FRotator(x, y, 0));


	if (!m_result.GetActor())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Name : aaa")));
		return;
	}

	shotFXComponent->SetNiagaraVariableVec3("BeamEnd", end2);

	//DrawDebugLine(GetWorld(), start, end, FColor::Orange, false, 0.1f);
	//name = "AttackLocation";

	if (m_result.GetActor()->ActorHasTag("Player"))
	{
		//bb->SetValueAsVector(name, hitr.Location);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Name : bbb")));
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Name : %s"), *hit.GetComponent()->GetName()));
		
		//auto temp = m_result.GetActor()->GetComponentByClass(StatsCompCls);
		//if (temp) {
		//	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("actor1 : %s"), *temp->GetName()));
		//	dynamic_cast<UStatsComponent*>(temp)->OnAttacked(shotDamage);
		//}

		TSet<UActorComponent*> components = m_result.GetActor()->GetComponents();
		for (UActorComponent* item : components) {

			//if (dynamic_cast<UStatsComponent*>(item)) {
			//	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("actor2 : %s"), *item->GetName()));
			//	dynamic_cast<UStatsComponent*>(item)->OnAttacked(shotDamage);

			//	cur_Recoil_X = max_Recoil_Radius;
			//	cur_Recoil_Y = max_Recoil_Radius;
			//}
			//else
			//{
			//	if (cur_Recoil_X >= min_Recoil_Radius)
			//	{
			//		cur_Recoil_X -= max_Recoil_Radius - min_Recoil_Radius / shot_MaxCount;
			//		cur_Recoil_Y -= max_Recoil_Radius - min_Recoil_Radius / shot_MaxCount;

			//		shot_MaxCount++;
			//	}
			//}
		}
	}

	shot = true;
}
