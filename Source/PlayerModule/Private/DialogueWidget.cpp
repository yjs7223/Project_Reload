// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget.h"
#include "Animation/WidgetAnimation.h"
#include "PlayerCharacter.h"
#include "Components/RichTextBlock.h"
#include "Engine/DataTable.h"
#include "DialogueScriptStruct.h"
#include "Kismet/KismetStringLibrary.h"
#include "UMG.h"


void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();
	lineLen = 0;
	nowLen = 0;
	writingSpeed = 0.05f;
	bHighlight = false;
}

void UDialogueWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UDialogueWidget::SetDialogueOpacity()
{
	
}

void UDialogueWidget::SetDialogueText(FName rowName)
{
	UDataTable* DialogueScripts = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/yjs/TextLog/DT_DialogueScripts.DT_DialogueScripts'"));
	if (DialogueScripts)
	{
		Dialogue_Overlay->SetRenderOpacity(1.0f);
		GetOwningPlayer()->GetWorldTimerManager().ClearTimer(WritingTimer);

		FDialogueRowStruct* dialogue = DialogueScripts->FindRow<FDialogueRowStruct>(rowName, FString("Row isn't exist"));
		Dialogue_Name->SetText(FText::FromString(dialogue->dialogue_Name));

		fullLine = dialogue->dialogue_Line;
		lineLen = dialogue->dialogue_Line.Len();
		nowLen = 1;

		FString line = UKismetStringLibrary::GetSubstring(fullLine, 0, nowLen);
		nowLen++;
		Dialogue_RichText->SetText(FText::FromString(line));

		GetOwningPlayer()->GetWorldTimerManager().SetTimer(WritingTimer, this, &UDialogueWidget::WriteDialogueText, writingSpeed, true);
	}
}

void UDialogueWidget::WriteDialogueText()
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

	Dialogue_RichText->SetText(FText::FromString(line));

	if (nowLen == lineLen)
	{
		GetOwningPlayer()->GetWorldTimerManager().ClearTimer(WritingTimer);
		PlayAnimationForward(DialogueAnimation);
	}
}
