// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Ammo_Widget.h"
#include "PlayerCharacter.h"
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


	texture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/yjs/UI/Textures/T_Infinite.T_Infinite'"));
	brush.SetResourceObject(texture);
	brush.SetImageSize(FVector2D(60.0f, 30.0f));
	Ammo_Infinite_image->SetBrush(brush);
	Ammo_Infinite_image->SetRenderOpacity(0.0f);

	//��Ʈ�����Ű��
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

	bWidgetVisible = true;
	widgetVisibleTime = 0;

	if (APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn()))
	{
		if (UPlayerWeaponComponent* MyWeaponComp = Cast<UPlayerWeaponComponent>(MyCharacter->weapon))
		{
			weapon = MyCharacter->weapon;
			//MyWeaponComp->OnVisibleAmmoUIDelegate.BindUObject(this, &UPlayer_Ammo_Widget::SetWidgetVisible);
			MyWeaponComp->OnChangedAmmoUIDelegate.BindUObject(this, &UPlayer_Ammo_Widget::UpdateAmmo);
		}
	}
}

void UPlayer_Ammo_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//UpdateAmmo();
	//SetWidgetVisible(Ammo_Overlay);
}

void UPlayer_Ammo_Widget::UpdateAmmo()
{
	if (weapon)
	{
		if (weapon->holdAmmo < 0)
		{
			Ammo_Infinite_image->SetRenderOpacity(1.0f);
			Max_Ammo_Text->SetRenderOpacity(0.0f);
		}
		else
		{
			Ammo_Infinite_image->SetRenderOpacity(0.0f);
			Max_Ammo_Text->SetRenderOpacity(1.0f);
		}
		Max_Ammo_Text->SetText(FText::FromString(FString::FromInt(weapon->holdAmmo)));
		Cur_Ammo_Text->SetText(FText::FromString(FString::FromInt(weapon->curAmmo)));
	}
}

void UPlayer_Ammo_Widget::SetWidgetVisible()
{
	/*if (weapon)
	{
		if (weapon->bAiming || weapon->bFire)
		{
			bWidgetVisible = true;
			Ammo_Overlay->SetRenderOpacity(1.0f);
		}
		else
		{
			bWidgetVisible = false;
			Ammo_Overlay->SetRenderOpacity(0.0f);
		}
	}*/
}
