// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraControllComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CoverComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "WeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "BaseInputComponent.h"

// Sets default values for this component's properties
UCameraControllComponent::UCameraControllComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/ATH/PlayerCharacter/DT_CameraControll.DT_CameraControll'"));
	if (DataTable.Succeeded())
	{
		m_CameraControllData = DataTable.Object;
	}
	
	// ...
}


// Called when the game starts
void UCameraControllComponent::BeginPlay()
{
	Super::BeginPlay();

	owner = dynamic_cast<ACharacter*>(GetOwner());
	
	m_Data = reinterpret_cast<FCameraControllData*>(m_CameraControllData->GetRowMap().begin().Value());

	m_Input = owner->FindComponentByClass<UBaseInputComponent>();
	m_Cover = owner->FindComponentByClass<UCoverComponent>();
	m_Weapon = owner->FindComponentByClass<UWeaponComponent>();
	m_FollowCamera = owner->FindComponentByClass<UCameraComponent>();
	m_FollowSpringArm = owner->FindComponentByClass<USpringArmComponent>();
	m_FollowSpringArm->SocketOffset = m_Data->camerapos;
	m_FollowSpringArm->SetRelativeLocation(FVector(0.0, 0.0, owner->GetDefaultHalfHeight() * 1.5));

}


// Called every frame
void UCameraControllComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector tempCamerapos = m_Data->camerapos;
	FRotator tempCameraRot = FRotator(0, 0, 0);
	tempCamerapos.Z -= owner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	if (m_Input && m_Input->getInput()->IsAiming) {
		m_Data->TargetArmLenght = m_Data->AimingArmLength;
		tempCamerapos += m_Data->camerapos_Aiming;
		tempCameraRot += m_Data->camerarot_aiming;
	}
	else {
		m_Data->TargetArmLenght = m_Data->IdleArmLength;
	}
	if (m_Cover && m_Cover->isPeeking()) {
		EPeekingState peekstate = m_Cover->getPeekingState();
		if (peekstate == EPeekingState::FrontRightStart) {
			tempCamerapos += m_Data->camerapos_FrontRightPeek;
			tempCameraRot += m_Data->camerarot_PeekingLRight;
		}
		else if (peekstate == EPeekingState::FrontLeftStart) {
			tempCamerapos += m_Data->camerapos_FrontLeftPeek;
			tempCameraRot += m_Data->camerarot_PeekingLeft;
		}
		else if (peekstate == EPeekingState::HighRightStart) {
			tempCamerapos += m_Data->camerapos_HighRightPeek;
			tempCameraRot += m_Data->camerarot_PeekingLRight;
		}
		else if (peekstate == EPeekingState::HighLeftStart) {
			tempCamerapos += m_Data->camerapos_HighLeftPeek;
			tempCameraRot += m_Data->camerarot_PeekingLeft;
		}
		else if (peekstate == EPeekingState::LowRightStart) {
			tempCamerapos += m_Data->camerapos_LowRightPeek;
			tempCameraRot += m_Data->camerarot_PeekingLRight;
		}
		else if (peekstate == EPeekingState::LowLeftStart) {
			tempCamerapos += m_Data->camerapos_LowLeftPeek;
			tempCameraRot += m_Data->camerarot_PeekingLeft;
		}

	}

	if (m_Cover->IsCover()) {
		if (m_Cover->FaceRight() > 0.0f) {
			tempCamerapos += m_Data->camerapos_FaceRight;
		}
		else {
			tempCamerapos += m_Data->camerapos_FaceLeft;
		}
	}


	if (owner->bIsCrouched) {
		tempCamerapos += m_Data->camerapos_Crouch;
		if (m_Cover->FaceRight() > 0.0f) {
			tempCamerapos += m_Data->camerapos_Crouch;
		}
		else {
			FVector tempvec = m_Data->camerapos_Crouch;
			tempvec.Y *= -1;
			tempCamerapos += tempvec;
		}
	}




	for (auto& item : m_Data->camerplusvectors)
	{
		tempCamerapos += item.Value;
	}

	m_FollowSpringArm->SocketOffset = FMath::VInterpTo(m_FollowSpringArm->SocketOffset, tempCamerapos, DeltaTime, m_Data->m_PosSpeed);
	m_FollowCamera->SetRelativeRotation(FMath::RInterpTo(m_FollowCamera->GetRelativeRotation(), tempCameraRot, DeltaTime, m_Data->m_RotSpeed));
	m_FollowSpringArm->TargetArmLength = FMath::FInterpTo(m_FollowSpringArm->TargetArmLength, m_Data->TargetArmLenght, DeltaTime, m_Data->m_LengthSpeed);
	//m_FollowSpringArm->SocketOffset = tempCamerapos;
	//m_FollowCamera->SetRelativeRotation(tempCameraRot);
	//m_FollowSpringArm->TargetArmLength = TargetArmLenght;

}



bool UCameraControllComponent::isAimChangeFinish()
{
	return m_FollowSpringArm->TargetArmLength >= m_Data->AimingArmLength - 1.f;
}
