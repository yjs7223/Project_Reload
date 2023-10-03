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

	owner = dynamic_cast<ACharacter*>(GetOwner());

	m_Input = owner->FindComponentByClass<UBaseInputComponent>();
	m_Cover = owner->FindComponentByClass<UCoverComponent>();
	m_Weapon = owner->FindComponentByClass<UWeaponComponent>();
	m_FollowCamera = owner->FindComponentByClass<UCameraComponent>();
	m_FollowSpringArm = owner->FindComponentByClass<USpringArmComponent>();
	m_Movement = owner->FindComponentByClass<UBaseCharacterMovementComponent>();

	InitArmLength = m_FollowSpringArm->TargetArmLength;
	InitFOV = m_FollowCamera->FieldOfView;

	//m_FollowSpringArm->SocketOffset = m_Data->camerapos;

	if (m_CameraControllStructData) {
		m_CameraControllStructData->foreach(
			[this](FString key, UCameraControllPakage* value) {
				value->NativeInitalize(owner);
				value->Initalize();
			});
	}

}


// Called every frame
void UCameraControllComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	int count = 1;
	FString tempKey = TEXT("Default");
	
	FCameraControllData tempControllData = 
		Cast<UCameraControllPakage>(m_CameraControllStructData->CameraControllMap[TEXT("Default")].GetDefaultObject())->Data;
	
	m_CameraControllStructData->foreach(
		[this, &tempControllData, &count, &tempKey](FString key, UCameraControllPakage* value)
		{
			if (value->IsControlled()) {
				tempControllData += value->Data;
				++count;

#if (UE_BUILD_DEBUG == 1)
				tempKey = FString::Printf(TEXT("%s, %s"), *tempKey, *key);
#else
#endif // UE_BUILD_DEBUG
			}
		});

#if (UE_BUILD_DEBUG == 1)
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("CameraState (%s)"), tempKey), true, true, FColor::Green, DeltaTime);
#else
#endif // UE_BUILD_DEBUG

	if (count > 1) {
		float inverseCount = 1.0f / (float)count;
		tempControllData.CameraLengthSpeed *= inverseCount;
		tempControllData.magnificationSpeed *= inverseCount;
		tempControllData.PosSpeed *= inverseCount;
		tempControllData.RotSpeed *= inverseCount;
	}

	m_FollowSpringArm->SocketOffset = FMath::VInterpTo(
		m_FollowSpringArm->SocketOffset, 
		tempControllData.camerapos, 
		DeltaTime, 
		tempControllData.PosSpeed);
	
	m_FollowCamera->SetRelativeRotation(FMath::RInterpTo(
		m_FollowCamera->GetRelativeRotation(), 
		tempControllData.camerarot, 
		DeltaTime, 
		tempControllData.RotSpeed));

	m_FollowCamera->FieldOfView = FMath::FInterpTo(
		m_FollowCamera->FieldOfView, 
		InitFOV - (InitFOV * (1 - tempControllData.magnification)),
		DeltaTime,
		tempControllData.magnificationSpeed);
	
	m_FollowSpringArm->TargetArmLength = FMath::FInterpTo(
		m_FollowSpringArm->TargetArmLength, 
		tempControllData.TargetArmLenght, 
		DeltaTime, 
		tempControllData.CameraLengthSpeed);
}

void UCameraControllPakage::NativeInitalize(ACharacter* _owner)
{
	owner = _owner;
}

void UCameraControllPakageDataAsset::foreach(TFunction<void(FString, UCameraControllPakage*)> fn)
{
	for (auto& item : CameraControllMap)
	{
		fn(item.Key, item.Value.GetDefaultObject());
	}
}

void FCameraControllData::operator+=(const FCameraControllData& other)
{
	camerapos += other.camerapos;
	camerarot += other.camerarot;
	TargetArmLenght += other.TargetArmLenght;
	magnification += other.magnification;

	PosSpeed += other.PosSpeed;
	RotSpeed += other.RotSpeed;
	CameraLengthSpeed += other.CameraLengthSpeed;
	magnificationSpeed += other.magnificationSpeed;
}