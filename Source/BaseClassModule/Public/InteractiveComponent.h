// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractiveComponent.generated.h"


DECLARE_DELEGATE(FOnInteractDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASECLASSMODULE_API UInteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractiveComponent();

	FOnInteractDelegate OnInteractDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ActiveInteractable();

	void StartInteract();

public:
	//상호작용 작동
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		bool bInteractive;

	//상호작용 활성화
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		bool bActive;


};
