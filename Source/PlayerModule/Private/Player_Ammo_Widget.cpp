// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Ammo_Widget.h"
#include "PlayerWeaponComponent.h"
#include "UMG.h"


void UPlayer_Ammo_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	UTexture2D* texture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/yjs/UI/Textures/T_Bullet_UI.T_Bullet_UI'"));
	FSlateBrush brush;
	brush.SetResourceObject(texture);
	brush.SetImageSize(FVector2D(48.0f, 48.0f));
	Ammo_Middle_image->SetBrush(brush);

	//폰트적용시키기
	FSlateFontInfo fontinfo;
	UFont* font = LoadObject<UFont>(NULL, TEXT("Font'/Game/yjs/UI/Fonts/Pretendard-Medium_Font.Pretendard-Medium_Font'"));
	fontinfo.FontObject = font;
	fontinfo.Size = 24;
	fontinfo.OutlineSettings = 1;
	Max_Ammo_Text->SetFont(fontinfo);

	font = LoadObject<UFont>(NULL, TEXT("Font'/Game/yjs/UI/Fonts/Pretendard-Bold_Font.Pretendard-Bold_Font'"));
	fontinfo.FontObject = font;
	fontinfo.Size = 34;
	fontinfo.OutlineSettings = 1;
	Cur_Ammo_Text->SetFont(fontinfo);
}

void UPlayer_Ammo_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateAmmo();
}

void UPlayer_Ammo_Widget::UpdateAmmo()
{
	if (weapon)
	{
		Max_Ammo_Text->SetText(FText::FromString(FString::FromInt(weapon->maxAmmo)));
		Cur_Ammo_Text->SetText(FText::FromString(FString::FromInt(weapon->curAmmo)));
	}
}
