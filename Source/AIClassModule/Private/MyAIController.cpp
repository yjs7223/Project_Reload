// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "ST_Range.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

AMyAIController::AMyAIController()
{

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_RangeDataObject(TEXT("DataTable'/Game/Aws/AI_Stat/DT_Move'"));
	if (DT_RangeDataObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
		DT_Range = DT_RangeDataObject.Object;
	}
}
