
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Perception/AISightTargetInterface.h"
#include "PlayerCharacter.generated.h"

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor = nullptr, const bool* bWasVisible = nullptr, int32* UserData = nullptr) const;

	void InitWidget(FViewport* viewport, uint32 value);
	void UpdateWidget(float deltatime);
	void WidgetShow();
	void CreateDamageWidget(float value, FHitResult result);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPlayerWeaponComponent* weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPlayerStatComponent* stat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* HPWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* AmmoWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> HP_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> Ammo_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> Crosshair_WidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCrosshair_Widget* Crosshair_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> Attacked_WidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAttacked_Widget* Attacked_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* CoverWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> Cover_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> Damage_Widget;

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
