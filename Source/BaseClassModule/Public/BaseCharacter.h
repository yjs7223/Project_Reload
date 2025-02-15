// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"


DECLARE_DELEGATE_OneParam(FOnSetDroneVisible, bool)

UCLASS()
class BASECLASSMODULE_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);
	//ABaseCharacter();

	FOnSetDroneVisible OnSetDroneVisible;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	static FName InputComponentName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Compoenet)
	class UCoverComponent* m_CoverComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCharacterSoundDataAsset* CharacterSound;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStatComponent* Stat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWeaponComponent* Weapon;*/

};
