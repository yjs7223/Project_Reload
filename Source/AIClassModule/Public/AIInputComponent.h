// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInputComponent.h"
#include "AIInputComponent.generated.h"

UENUM(BlueprintType)
enum class AI_Anim_Type : uint8
{
	Crouching,

	Runing,

	Fire,

	Aiming,

	Reload,

	Cover,
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AICLASSMODULE_API UAIInputComponent : public UBaseInputComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);
public:
	//디버그용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIInput)
		bool AI_Crouch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIInput)
		bool AI_Cover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIInput)
		bool AI_Running;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIInput)
		bool AI_Fire;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIInput)
		bool AI_Aiming;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIInput)
		bool AI_Reload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIInput)
		class UBaseCharacterMovementComponent* movement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIInput)
		class UCoverComponent* covercomp;
public:
	UFUNCTION(BlueprintCallable, Category = "AIAnim")
	void AIMoveForward(float Value);
	UFUNCTION(BlueprintCallable, Category = "AIAnim")
	void AIMoveRight(float Value);


	UFUNCTION(BlueprintCallable, Category = "AIAnim")
		void AICrouching();
	UFUNCTION(BlueprintCallable, Category = "AIAnim")
		void AIStopCrouching();


	UFUNCTION(BlueprintCallable, Category = "AIAnim")
		void AIRuning();
	UFUNCTION(BlueprintCallable, Category = "AIAnim")
		void AIStopRuning();


	UFUNCTION(BlueprintCallable, Category = "AIAnim")
	void AIStartFire();
	UFUNCTION(BlueprintCallable, Category = "AIAnim")
	void AIStopFire();

	UFUNCTION(BlueprintCallable, Category = "AIAnim")
	void AIStartAiming();
	UFUNCTION(BlueprintCallable, Category = "AIAnim")
	void AIStopAiming();

	UFUNCTION(BlueprintCallable, Category = "AIAnim")
	void AIStartReload();

	UFUNCTION(BlueprintCallable, Category = "AIAnim")
		void AIStopReload();


	UFUNCTION(BlueprintCallable, Category = "AIAnim")
		void AIStartCover();
	UFUNCTION(BlueprintCallable, Category = "AIAnim")
		void AIStopCover();
};
