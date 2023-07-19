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

	AimingArmLength = 90.0f;
	IdleArmLength = 120.0f;
	TargetArmLenght = IdleArmLength;
	
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
	m_FollowSpringArm->SocketOffset = camerapos;
	m_FollowSpringArm->SetRelativeLocation(owner->GetMesh()->GetBoneLocation(TEXT("spine_01"), EBoneSpaces::ComponentSpace));
}


// Called every frame
void UCameraControllComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector tempCamerapos = camerapos;
	FRotator tempCameraRot = FRotator(0, 0, 0);
	tempCamerapos.Z -= owner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	if (m_Input && m_Input->getInput()->IsAiming) {
		TargetArmLenght = AimingArmLength;
		tempCamerapos += camerapos_Aiming;
		tempCameraRot += camerarot_aiming;
	}
	else {
		TargetArmLenght = IdleArmLength;
	}
	if (m_Cover && m_Cover->isPeeking()) {
		EPeekingState peekstate = m_Cover->getPeekingState();
		if (peekstate == EPeekingState::FrontRightStart) {
			tempCamerapos += camerapos_FrontRightPeek;
			tempCameraRot += camerarot_PeekingLRight;
		}
		else if (peekstate == EPeekingState::FrontLeftStart) {
			tempCamerapos += camerapos_FrontLeftPeek;
			tempCameraRot += camerarot_PeekingLeft;
		}
		else if (peekstate == EPeekingState::HighRightStart) {
			tempCamerapos += camerapos_HighRightPeek;
			tempCameraRot += camerarot_PeekingLRight;
		}
		else if (peekstate == EPeekingState::HighLeftStart) {
			tempCamerapos += camerapos_HighLeftPeek;
			tempCameraRot += camerarot_PeekingLeft;
		}
		else if (peekstate == EPeekingState::LowRightStart) {
			tempCamerapos += camerapos_LowRightPeek;
			tempCameraRot += camerarot_PeekingLRight;
		}
		else if (peekstate == EPeekingState::LowLeftStart) {
			tempCamerapos += camerapos_LowLeftPeek;
			tempCameraRot += camerarot_PeekingLeft;
		}

	}

	if (m_Cover->IsCover()) {
		if (m_Cover->FaceRight() > 0.0f) {
			tempCamerapos += camerapos_FaceRight;
		}
		else {
			tempCamerapos += camerapos_FaceLeft;
		}
	}


	if (owner->bIsCrouched) {
		tempCamerapos += camerapos_Crouch;
		if (m_Cover->FaceRight() > 0.0f) {
			tempCamerapos += camerapos_Crouch;
		}
		else {
			FVector tempvec = camerapos_Crouch;
			tempvec.Y *= -1;
			tempCamerapos += tempvec;
		}
	}




	for (auto& item : camerplusvectors)
	{
		tempCamerapos += item.Value;
	}

	m_FollowSpringArm->SocketOffset = FMath::VInterpTo(m_FollowSpringArm->SocketOffset, tempCamerapos, DeltaTime, m_PosSpeed);
	m_FollowCamera->SetRelativeRotation(FMath::RInterpTo(m_FollowCamera->GetRelativeRotation(), tempCameraRot, DeltaTime, m_RotSpeed));
	m_FollowSpringArm->TargetArmLength = FMath::FInterpTo(m_FollowSpringArm->TargetArmLength, TargetArmLenght, DeltaTime, m_LengthSpeed);
	//m_FollowSpringArm->SocketOffset = tempCamerapos;
	//m_FollowCamera->SetRelativeRotation(tempCameraRot);
	//m_FollowSpringArm->TargetArmLength = TargetArmLenght;

}



bool UCameraControllComponent::isAimChangeFinish()
{
	return m_FollowSpringArm->TargetArmLength >= AimingArmLength - 1.f;
}
