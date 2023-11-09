// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "NiagaraComponent.h"
#include "Components/SceneComponent.h"
#include "MascotDrone.generated.h"

UCLASS()
class DRONEMODULE_API AMascotDrone : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMascotDrone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetDroneVisable(bool flag);
public:

	//Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Component")
	class UFloatingPawnMovement* m_FloatingCmp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	 TArray <UStaticMeshComponent*> m_StaticMeshCmp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		class USceneComponent* m_Empcmp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		 TArray <UNiagaraComponent*> m_EmpEffect;

public:
	//SphereCollisionTrigger

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		class USphereComponent* CollisionMesh;

	UFUNCTION()
		void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "variable")
	TArray<AActor*> m_NearAI;

public:
	//Skill
	UFUNCTION(BlueprintCallable, Category = "MascotDrone")
		TArray<AActor*> EMP();

	bool EmpDelay = false;

};
