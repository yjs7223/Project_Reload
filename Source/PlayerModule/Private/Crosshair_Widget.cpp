// Fill out your copyright notice in the Description page of Project Settings.


#include "Crosshair_Widget.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetTree.h"
#include "PlayerCharacter.h"
#include "PlayerWeaponComponent.h"
#include "UMG.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"


UCrosshair_Widget::UCrosshair_Widget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	
}

void UCrosshair_Widget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
}

void UCrosshair_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	m_alpha = 0;
	m_offset = 0;
	m_hitTime = 0;

	UTexture2D* texture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/yjs/UI/Textures/CrossHair_Texture/T_CrossHair_NormalHit.T_CrossHair_NormalHit'"));
	FSlateBrush brush;
	
	brush.SetResourceObject(texture);
	UOverlaySlot* hitslot;
	if (Hit_image)
	{
		hitslot = Cast<UOverlaySlot>(Hit_image->Slot);
		hitslot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		hitslot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);

		brush.SetImageSize(FVector2D(90.f, 90.f));
		Hit_image->SetBrush(brush);
		//Hit_image->SetRenderScale(FVector2D(3.0f, 3.0f));
		//Hit_image->SetRenderTransformAngle(45.0f);
		Hit_image->SetRenderOpacity(0.0f);
	}

	/*if (HeadHit_image)
	{
		hitslot = Cast<UOverlaySlot>(HeadHit_image->Slot);
		hitslot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		hitslot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

		brush.TintColor = FSlateColor(FColor::Red);
		HeadHit_image->SetBrush(brush);
		HeadHit_image->SetRenderScale(FVector2D(3.0f, 3.0f));
		HeadHit_image->SetRenderTransformAngle(45.0f);
		HeadHit_image->SetRenderOpacity(0.0f);
	}*/
	
	UMaterialInterface* mat = LoadObject<UMaterialInterface>(NULL, TEXT("Material'/Game/yjs/UI/Materials/M_AmmoRadial.M_AmmoRadial'"));
	if (mat)
	{
		AmmoMat = UMaterialInstanceDynamic::Create(mat, this);
		if (AmmoMat)
		{
			AmmoMat->SetScalarParameterValue(FName(TEXT("Percent")), 1.0f);
			Cross_Ammo_Image->SetBrushFromMaterial(AmmoMat);
			Cross_Ammo_Image->SetRenderTranslation(FVector2D(16.0f, 16.0f));
		}
	}

	if (APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn()))
	{
		if (UPlayerWeaponComponent* MyWeaponComp = Cast<UPlayerWeaponComponent>(MyCharacter->weapon))
		{
			weapon = MyCharacter->weapon;
			MyWeaponComp->OnChangedCrossHairAmmoDelegate.BindUObject(this, &UCrosshair_Widget::SetAmmoImage);
			MyWeaponComp->OnChangedCrossHairHitDelegate.BindUObject(this, &UCrosshair_Widget::CheckHit);
			MyWeaponComp->OnVisibleCrossHairUIDelegate.BindUObject(this, &UCrosshair_Widget::SetWidgetVisible);
		}
	}

	bWidgetVisible = true;
	widgetVisibleTime = 0;
}

void UCrosshair_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(m_alpha));
	UpdateCrosshair(InDeltaTime);
}

void UCrosshair_Widget::UpdateCrosshair(float DeltaTime)
{
	if (weapon)
	{
		if (CalcAlphaValue(DeltaTime))
		{
			weapon->m_firecount = 0;
		}
		CalcOffset(weapon->m_spreadPower);
		SetCrosshairTranslation();
		//SetAmmoImage();
	}
}

void UCrosshair_Widget::SetCrosshairTranslation()
{
	FVector2D up = FVector2D(0, -m_offset);
	Up_Cross_image->SetRenderTranslation(up);

	FVector2D down = FVector2D(0, m_offset);
	Down_Cross_image->SetRenderTranslation(down);

	FVector2D left = FVector2D(-m_offset, 0);
	Left_Cross_image->SetRenderTranslation(left);

	FVector2D right = FVector2D(m_offset, 0);
	Right_Cross_image->SetRenderTranslation(right);

	FVector2D ammo = FVector2D(m_offset * 0.3f + 16.0f, m_offset * 0.3f + 16.0f);
	Cross_Ammo_Image->SetRenderTranslation(ammo);
}

void UCrosshair_Widget::CalcOffset(float spreadpower)
{
	m_offset = FMath::Lerp(0.0f, spreadpower * 300.0f, m_alpha);
}

bool UCrosshair_Widget::CalcAlphaValue(float DeltaTime)
{
	if (weapon->bRecoil)
	{
		m_alpha += DeltaTime * 0.6f;
	}
	else
	{
		m_alpha -= DeltaTime * 0.4f;
	}

	if (m_alpha < 0)
	{
		m_alpha = 0;
		return true;
	}
	else if (m_alpha > 1)
	{
		m_alpha = 1;
	}
	return false;
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(m_alpha));
}

void UCrosshair_Widget::CheckHit()
{
	GetWorld()->GetTimerManager().ClearTimer(HitTimer);
	weapon->m_result.Location;
	FVector2D loc;
	FIntVector2 screensize;
	UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), weapon->m_result.Location, loc);
	GetOwningPlayer()->GetViewportSize(screensize.X, screensize.Y);
	loc.X -= screensize.X/2;
	loc.Y -= screensize.Y/2;
	Dot_image->SetRenderTranslation(loc);

	if (weapon->isHit)
	{
		if (weapon->headhit)
		{
			weapon->isHit = false;
			weapon->headhit = false;
			Hit_image->SetBrushTintColor(FSlateColor(FColor::Red));
			Hit_image->SetRenderOpacity(1.0f);
		}
		else
		{
			weapon->isHit = false;
			Hit_image->SetBrushTintColor(FSlateColor(FColor::White));
			Hit_image->SetRenderOpacity(1.0f);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(HitTimer,
		FTimerDelegate::CreateLambda([&]()
		{
			Dot_image->SetRenderTranslation(FVector2D(.0f, .0f));
			Hit_image->SetRenderOpacity(0.0f);
		}
	), .5f, false);
}

void UCrosshair_Widget::SetAmmoImage()
{
	float ammovalue = 0;

	switch (weapon->weapontype)
	{
	case EWeaponType::TE_Pistol:
		ammovalue = weapon->curAmmo / 10.0f;
		break;
	case EWeaponType::TE_Rifle:
		ammovalue = weapon->curAmmo / 30.0f;
		break;
	case EWeaponType::TE_Shotgun:
		ammovalue = weapon->curAmmo / 7.0f;
		break;
	default:
		ammovalue = weapon->curAmmo / 30.0f;
		break;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(ammovalue));
	AmmoMat->SetScalarParameterValue(FName(TEXT("Percent")), ammovalue);
	Cross_Ammo_Image->SetBrushFromMaterial(AmmoMat);
}

void UCrosshair_Widget::SetWidgetVisible()
{
	GetWorld()->GetTimerManager().ClearTimer(VisibleTimer);
	StopAllAnimations();
	Crosshair_Overlay->SetRenderOpacity(1.0f);
	if (weapon)
	{
		if (weapon->isAiming || weapon->isFire)
		{
			bWidgetVisible = true;
			GetWorld()->GetTimerManager().ClearTimer(VisibleTimer);
			Crosshair_Overlay->SetRenderOpacity(1.0f);
		}
		else if(!weapon->isAiming && !weapon->isFire)
		{
			bWidgetVisible = false;
			GetWorld()->GetTimerManager().SetTimer(VisibleTimer, FTimerDelegate::CreateLambda([&]()
				{
					if (FadeOutAnim)
					{
						PlayAnimationForward(FadeOutAnim);
					}
				}), 5.f, false);
		}
	}
}

