
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Perception/AISightTargetInterface.h"
#include "PlayerCharacter.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnVisibleAllUIDelegate);

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API APlayerCharacter : public ABaseCharacter, public IAISightTargetInterface
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

	FOnVisibleAllUIDelegate OnVisibleAllUIDelegate;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor = nullptr, const bool* bWasVisible = nullptr, int32* UserData = nullptr) const;

	//static void Play

	void InitWidget(FViewport* viewport, uint32 value);
	void UpdateWidget(float deltatime);
	void WidgetShow();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPlayerWeaponComponent* weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPlayerStatComponent* stat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UPlayerHUDWidget> PlayerHUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPlayerHUDWidget* PlayerHUDWidget;

	FTimerHandle DamageTimer;
    
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* m_FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* m_FollowSpringArm;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Compoenet)
	class UPlayerMoveComponent* m_PlayerMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UCameraControllComponent* m_CameraControll;

};
