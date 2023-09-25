// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionWidget.h"
#include "Animation/WidgetAnimation.h"
#include "PlayerCharacter.h"
#include "Components/RichTextBlock.h"
#include "Engine/DataTable.h"
#include "MissionScriptStruct.h"
#include "Kismet/KismetStringLibrary.h"
#include "UMG.h"



void UMissionWidget::NativeConstruct()
{
	lineLen = 0;
	nowLen = 0;
	writingSpeed = 0.05f;
	bHighlight = false;
}

void UMissionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
}

void UMissionWidget::SetMissionText(FName rowName)
{
	UDataTable* DialogueScripts = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/yjs/TextLog/DT_MissionScripts.DT_MissionScripts'"));
	if (DialogueScripts)
	{
		GetOwningPlayer()->GetWorldTimerManager().ClearTimer(WritingTimer);

		FMissionRowStruct* mission = DialogueScripts->FindRow<FMissionRowStruct>(rowName, FString("Row isn't exist"));
		Mission_Name->SetText(FText::FromString(mission->mission_Name));

		fullLine = mission->mission_Desc;
		lineLen = mission->mission_Desc.Len();
		nowLen = 1;

		FString line = UKismetStringLibrary::GetSubstring(fullLine, 0, nowLen);
		nowLen++;
		Mission_RichText->SetText(FText::FromString(line));

		GetOwningPlayer()->GetWorldTimerManager().SetTimer(WritingTimer, this, &UMissionWidget::WriteMissionText, writingSpeed, true);
	}
}

void UMissionWidget::WriteMissionText()
{
	FString line = UKismetStringLibrary::GetSubstring(fullLine, 0, nowLen);
	if (fullLine[nowLen] == '<' && !bHighlight)
	{
		bHighlight = true;
		nowLen += 12;
		line += "<Highlight>";
		line += fullLine[nowLen];
	}
	else if (fullLine[nowLen] == '<' && bHighlight)
	{
		bHighlight = false;
		nowLen += 4;
		line += "</>";
		line += fullLine[nowLen];
	}
	else
	{
		nowLen++;
	}

	if (bHighlight)
	{
		line += "</>";
	}

	Mission_RichText->SetText(FText::FromString(line));

	if (nowLen == lineLen)
	{
		GetOwningPlayer()->GetWorldTimerManager().ClearTimer(WritingTimer);
	}
}
