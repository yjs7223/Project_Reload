// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraControllComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CoverComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "WeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "BaseCharacterMovementComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include "BaseInputComponent.h"

// Sets default values for this component's properties
UCameraControllComponent::UCameraControllComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCameraControllComponent::BeginPlay()
{
	Super::BeginPlay();

	//m_PlayerCharacter = Cast<ACharacter>(GetOwner());
	m_PlayerCharacter = GetOwner<APlayerController>()->GetPawn<ACharacter>();

	m_Input = m_PlayerCharacter->FindComponentByClass<UBaseInputComponent>();
	m_Cover = m_PlayerCharacter->FindComponentByClass<UCoverComponent>();
	m_Weapon = m_PlayerCharacter->FindComponentByClass<UWeaponComponent>();
	m_FollowCamera = m_PlayerCharacter->FindComponentByClass<UCameraComponent>();
	m_FollowSpringArm = m_PlayerCharacter->FindComponentByClass<USpringArmComponent>();
	m_Movement = m_PlayerCharacter->FindComponentByClass<UBaseCharacterMovementComponent>();

	m_DefaultPos = m_FollowSpringArm->SocketOffset;
	m_DefaultRot = m_FollowCamera->GetRelativeRotation();
	m_DefaultArmLength = m_FollowSpringArm->TargetArmLength;
	m_DefaultMagnification = 1.0f;
	
	InitFOV = m_FollowCamera->FieldOfView;

	//m_FollowSpringArm->SocketOffset = m_Data->camerapos;

	if (m_CameraControllStructData) {
		m_CameraControllStructData->foreach(
			[this](FString key, UCameraControllPakage* value) {
				value->NativeInitalize(m_PlayerCharacter);
				value->Initalize();
			});
	}
	m_CameraControllStructData->m_FaceRight.Initalize();
	m_CameraControllStructData->m_Crouch.Initalize();

}


void UCameraControllComponent::SetDebugMode(bool isEnable)
{
	DebugMode = isEnable;
}

// Called every frame
void UCameraControllComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString tempKey = TEXT("");
	
	FVector defaultPos = m_DefaultPos;
	FRotator defaultRot = m_DefaultRot;
	float defaultArmLength = m_DefaultArmLength;
	float defaultMagnification = m_DefaultMagnification;

	for (auto& item : m_CameraControllStructData->CameraControllMap)
	{
		UCameraControllPakage* value = item.Value.GetDefaultObject();

		bool iscontroll = value->IsControlled();
		value->Data.Temp(DeltaTime, iscontroll);
		defaultPos += value->Data.camerapos.CurrentValue;
		defaultRot += value->Data.camerarot.CurrentValue;
		defaultArmLength += value->Data.SpringArmLength.CurrentValue;
		defaultMagnification += value->Data.magnification.CurrentValue;
		if (iscontroll && DebugMode) {
			tempKey = FString::Printf(TEXT("%s, %s"), *tempKey, *item.Key);
		}
	}

	if (DebugMode) {
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("CameraState (%s)"), *tempKey), true, true, FColor::Green, DeltaTime);
	}

	m_FollowSpringArm->SocketOffset = defaultPos;
	m_FollowCamera->SetRelativeRotation(defaultRot);
	m_FollowSpringArm->TargetArmLength = defaultArmLength;
	m_FollowCamera->FieldOfView = InitFOV - (InitFOV * (defaultMagnification - 1));

	m_CameraControllStructData->m_FaceRight.Easing(m_Cover->IsFaceRight() ? -DeltaTime : DeltaTime);
	m_FollowSpringArm->SocketOffset.Y = UKismetMathLibrary::Ease(
		defaultPos.Y,
		-defaultPos.Y, 
		m_CameraControllStructData->m_FaceRight.time,
		m_CameraControllStructData->m_FaceRight.posEaseType);

	m_CameraControllStructData->m_Crouch.Easing(m_PlayerCharacter->bIsCrouched ? -DeltaTime : DeltaTime);
	m_FollowSpringArm->TargetOffset.Z = UKismetMathLibrary::Ease(
		m_PlayerCharacter->GetDefaultHalfHeight(),
		m_PlayerCharacter->GetDefaultHalfHeight() * 2.0f,
		m_CameraControllStructData->m_Crouch.time,
		m_CameraControllStructData->m_Crouch.posEaseType);
}

void UCameraControllPakage::NativeInitalize(ACharacter* _m_PlayerCharacter)
{
	m_PlayerCharacter = _m_PlayerCharacter;
	Data.camerapos.Initalize();
	Data.camerarot.Initalize();
	Data.SpringArmLength.Initalize();
	Data.magnification.Initalize();
}

void UCameraControllPakageDataAsset::foreach(TFunction<void(FString, UCameraControllPakage*)> fn)
{
	for (auto& item : CameraControllMap)
	{
		fn(item.Key, item.Value.GetDefaultObject());
	}
}

void FCameraControllDataElement::Initalize()
{
	if (blendIn != 0) {
		InvblendIn = 1.0f / blendIn ;
	}
	else {
		InvblendIn = FLT_MAX;
	}
	if (blendOut != 0) {
		InvblendOut = 1.0f / blendOut;
	}
	else {
		InvblendOut = FLT_MAX;
	}
}

void FCameraControllDataElement::Easing(float DeltaTime)
{
	if (DeltaTime > 0) {
		time = FMath::Clamp(time + (DeltaTime * InvblendIn), 0.0f, 1.0f);
	}
	else {
		time = FMath::Clamp(time + (DeltaTime * InvblendOut), 0.0f, 1.0f);
	}
}

void FCameraControllDataElementReal::Easing(float DeltaTime)
{
	Super::Easing(DeltaTime);
	CurrentValue = UKismetMathLibrary::Ease(0, TargetValue, time, posEaseType);
}

void FCameraControllDataElementVector::Easing(float DeltaTime)
{
	Super::Easing(DeltaTime);
	CurrentValue = UKismetMathLibrary::VEase(FVector::ZeroVector, TargetValue, time, posEaseType);
}

void FCameraControllDataElementRotater::Easing(float DeltaTime)
{
	Super::Easing(DeltaTime);
	CurrentValue = UKismetMathLibrary::REase(FRotator::ZeroRotator, TargetValue, time, true, posEaseType);
}

FCameraControllData::FCameraControllData()
{

}

void FCameraControllData::Temp(float DeltaTime, bool isChecked)
{
	float deltatime = isChecked ? DeltaTime : -DeltaTime;
	camerapos.Easing(deltatime);
	camerarot.Easing(deltatime);
	SpringArmLength.Easing(deltatime);
	magnification.Easing(deltatime);
}
