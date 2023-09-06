// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneAICharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Engine/CollisionProfile.h"	
#include "UObject/Object.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

#include "DroneAIController.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
ADroneAICharacter::ADroneAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Ÿ�Ӷ��� �ʱ�ȭ�� ���ε�
	LerpDroneMoveToTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineFront"));
	LerpDroneMoveToFunction.BindUFunction(this, FName("LerpDroneMoveTo"));
	LerpDroneMoveTimelineFinish.BindUFunction(this, FName("LerpDroneMoveFinish"));


	//ĳ���� �����Ʈ ����
	//CharMovement = FindComponentByClass(UCharacterMovementComponent::StaticClass());
	CharMovement = GetCharacterMovement();
	CharMovement->DefaultLandMovementMode = EMovementMode::MOVE_Flying;
	CharMovement->MaxFlySpeed = 400.f;


	//ĸ�� ������ ���� �ȵ�?
	//GetCapsuleComponent()->SetCapsuleRadius(30);
	GetCapsuleComponent()->SetCapsuleHalfHeight(35.0f);
	
	//�޽� �߰�
	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMesh"));
	m_mesh->SetupAttachment(GetMesh(), TEXT("DroneMesh"));

}

// Called when the game starts or when spawned
void ADroneAICharacter::BeginPlay()
{
	Super::BeginPlay();

	MoveToTimelineSetting();

	m_player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//Cast<ADroneAIController>(GetController())->GetBlackboardComponent()->SetValueAsObject("Player", m_player);

}

// Called every frame
void ADroneAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADroneAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void ADroneAICharacter::LerpDroneMoveTo(float Value)
{

	FVector a = m_player->GetActorLocation()
		, b = Owner->GetActorLocation();

	DrawDebugLine(GetWorld(), a, b, FColor::Green);
	a.Z = 0;
	b.Z = 0;

	//�����̼� ����
	FVector c = FMath::Lerp(b, a, Value/3);
		
	c.Z = Owner->GetActorLocation().Z;
	
	SetActorLocation(c);

	//�������� ����
	FRotator moveRot = FMath::Lerp
	(
		this->GetActorRotation(), 
		UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), m_player->GetActorLocation()) 
		,Value/3
	);
	FRotator rr = FRotator(0, moveRot.Yaw, 0);
	
	
	//������ ��Ʋ���°� ����
	if(Value<2.999f)
		SetActorRotation(rr);

	//�����
	DrawDebugBox(GetWorld(), c, FVector(30, 30, 30), rr.Quaternion(), FColor::Red);
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::SanitizeFloat(Value));
	IsMoving = true;
}

void ADroneAICharacter::LerpDroneMoveFinish()
{
	//����ɶ� ȣ�� �Լ�
	IsMoving = false;
}

void ADroneAICharacter::MoveToTimelineSetting()
{
	if (LerpDroneMoveToFloat)
	{
		LerpDroneMoveToTimeline->AddInterpFloat(LerpDroneMoveToFloat, LerpDroneMoveToFunction);
		LerpDroneMoveToTimeline->SetTimelineFinishedFunc(LerpDroneMoveTimelineFinish);
		LerpDroneMoveToTimeline->SetLooping(false);
	}
}




void ADroneAICharacter::StartMoveTo()
{
	if(!IsMoving)
		LerpDroneMoveToTimeline->Play();

	//OnSuceess.Broadcast(-228,"tset");

	RemoveFromRoot();
	//return IsMoving;
}
