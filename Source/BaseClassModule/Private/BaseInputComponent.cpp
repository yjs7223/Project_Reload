// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInputComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UBaseInputComponent::UBaseInputComponent() : m_inputType(EInputType::None)
{

}


// Called when the game starts
void UBaseInputComponent::BeginPlay()
{
	Super::BeginPlay();

	if (m_inputType == EInputType::Setting) return;

	
	m_inputData = MakeShared<FInputData>();
	owner = Cast<ACharacter>(GetOwner());
	TObjectPtr<class UInputComponent> InputComponent = owner->InputComponent;
	
	InputComponent->BindAxis("MoveForward", this, &UBaseInputComponent::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &UBaseInputComponent::MoveRight);

	InputComponent->BindAxis("Turn", owner, &ACharacter::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", owner, &ACharacter::AddControllerPitchInput);
	InputComponent->BindAction("Crouch", IE_Pressed, this, &UBaseInputComponent::Crouching);

	InputComponent->BindAction("Fire", IE_Pressed, this, &UBaseInputComponent::StartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &UBaseInputComponent::StopFire);

	InputComponent->BindAction("Aiming", IE_Pressed, this, &UBaseInputComponent::StartAiming);
	InputComponent->BindAction("Aiming", IE_Released, this, &UBaseInputComponent::StopAiming);

	InputComponent->BindAction("Reload", IE_Pressed, this, &UBaseInputComponent::StartReload);

	InputComponent->BindAction("Cover", IE_Pressed, this, &UBaseInputComponent::CoverInputEvent);

	InputComponent->BindAction("Aiming", IE_Pressed, this, &UBaseInputComponent::StartPeeking);
	InputComponent->BindAction("Aiming", IE_Released, this, &UBaseInputComponent::StopPeeking);
}

void UBaseInputComponent::SetInputPtr(TSharedPtr<FInputData> input_ptr)
{
	m_inputData = input_ptr;
}

void UBaseInputComponent::MoveForward(float Value)
{
	m_inputData->movevec.X = Value;
}

void UBaseInputComponent::MoveRight(float Value)
{
	m_inputData->movevec.X = Value;
}

void UBaseInputComponent::Runing()
{
	m_inputData->IsRuning ? m_inputData->IsRuning = false : m_inputData->IsRuning = true;
}

void UBaseInputComponent::Crouching()
{
	if (owner->CanCrouch()) {
		owner->Crouch();
	}
	else {
		if (!mCanUnCrouch) return;
		owner->UnCrouch();
	}
}

void UBaseInputComponent::StartFire()
{
	m_inputData->IsFire = true;
}

void UBaseInputComponent::StopFire()
{
	m_inputData->IsFire = false;
}

void UBaseInputComponent::StartAiming()
{
	m_inputData->IsAiming = true;
}

void UBaseInputComponent::StopAiming()
{
	m_inputData->IsAiming = false;
}

void UBaseInputComponent::StartReload()
{
	m_inputData->IsReload = true;
}

void UBaseInputComponent::CoverInputEvent()
{
	//m_inputData->CoverInputEventDelegate.ExecuteIfBound();
}

void UBaseInputComponent::StartPeeking()
{
	//
}

void UBaseInputComponent::StopPeeking()
{
	//
}
