// Fill out your copyright notice in the Description page of Project Settings.


#include "AISensingComponent.h"
#include "Engine/DataTable.h"
#include "ST_Range.h"

UAISensingComponent::UAISensingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ������ ���̺� ����
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/Aws/AI_Stat/DT_Range.DT_Range'"));
	if (DataTable.Succeeded())
	{
		AIRangeData = DataTable.Object;
	}

	// ������ ������ ��������
	curAIRangeData = AIRangeData->FindRow<FST_Range>("Rifle_E", TEXT(""));
}


void UAISensingComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UAISensingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


float UAISensingComponent::CalculateSectorArea(float centralAngle, float radius)
{
	float thetaRadians = centralAngle * PI / 180.0f;
	float area = (thetaRadians / (2 * PI)) * PI * radius * radius;

	return area;
}

bool UAISensingComponent::ShotSenseRange()
{
	// ���� ª�� �ĸ� �������� üũ
	if (CalculateSectorArea(curAIRangeData->AimBwd_Angle, curAIRangeData->AimBwd_Radius))
	{
		return true;
	}
	// ����
	else if (CalculateSectorArea(curAIRangeData->AimSide_Angle, curAIRangeData->AimSide_Radius))
	{
		return true;
	}
	// ����
	else if (CalculateSectorArea(curAIRangeData->AimFwd_Angle, curAIRangeData->AimFwd_Radius))
	{
		return true;
	}

	return false;
}
