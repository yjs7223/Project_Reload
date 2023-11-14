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
#include "StatComponent.h"
#include "CharacterSoundDataAsset.h"

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
	bBlocking = false;

	Crosshair_Overlay->SetRenderOpacity(1.0f);
	Reload_Overlay->SetRenderOpacity(0.0f);
	Hit_Overlay->SetRenderOpacity(0.0f);
	Kill_Overlay->SetRenderOpacity(0.0f);
	LowAmmoText->SetRenderOpacity(0.0f);

	UTexture2D* texture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/yjs/UI/Textures/CrossHair_Texture/T_CrossHair_NormalHit.T_CrossHair_NormalHit'"));
	FSlateBrush brush;
	
	UMaterialInterface* mat = LoadObject<UMaterialInterface>(NULL, TEXT("Material'/Game/yjs/UI/Materials/M_AmmoRadial.M_AmmoRadial'"));
	if (mat)
	{
		AmmoMat = UMaterialInstanceDynamic::Create(mat, this);
		if (AmmoMat)
		{
			AmmoMat->SetScalarParameterValue(FName(TEXT("Percent")), 1.0f);
			Cross_Ammo_Image->SetBrushFromMaterial(AmmoMat);
			Cross_Ammo_Image->SetRenderTranslation(FVector2D(12.0f, 12.0f));
		}
	}

	if (APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn()))
	{
		if (UPlayerWeaponComponent* MyWeaponComp = MyCharacter->FindComponentByClass<UPlayerWeaponComponent>())
		{
			weapon = MyWeaponComp;
			MyWeaponComp->OnChangedCrossHairAmmoDelegate.BindUObject(this, &UCrosshair_Widget::SetAmmoImage);
			MyWeaponComp->OnChangedCrossHairHitDelegate.BindUObject(this, &UCrosshair_Widget::MoveDot);
			MyWeaponComp->OnChangedCrossHairDieDelegate.BindUObject(this, &UCrosshair_Widget::CheckDie);
			//MyWeaponComp->OnVisibleCrossHairUIDelegate.BindUObject(this, &UCrosshair_Widget::SetWidgetVisible);
			MyWeaponComp->OnPlayReloadUIDelegate.BindUObject(this, &UCrosshair_Widget::PlayReloadAnim);
			MyWeaponComp->OnStopReloadUIDelegate.BindUObject(this, &UCrosshair_Widget::StopReloadAnim);
			MyWeaponComp->OnBlockingUIDelegate.BindUObject(this, &UCrosshair_Widget::Blocking);
			//MyCharacter->OnVisibleAllUIDelegate.AddUObject(this, &UCrosshair_Widget::SetWidgetVisible);
		}
	}

	SetCombatWidgetVisible();
	bWidgetVisible = true;
	widgetVisibleTime = 0;
}

void UCrosshair_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(m_alpha));
	UpdateCrosshair(InDeltaTime);
}

void UCrosshair_Widget::NativeDestruct()
{
	GetWorld()->GetTimerManager().ClearTimer(DotTimer);
	GetWorld()->GetTimerManager().ClearTimer(VisibleTimer);
	GetWorld()->GetTimerManager().ClearTimer(HitTimer);
	GetWorld()->GetTimerManager().ClearTimer(KillTimer);
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);

	Super::NativeDestruct();
}

void UCrosshair_Widget::UpdateCrosshair(float DeltaTime)
{
	if (weapon)
	{
		if (CalcAlphaValue(DeltaTime))
		{
			weapon->m_firecount = 0;
		}
		if(weapon->bAiming)
		{ 
			CalcOffset(weapon->m_spreadPower * weapon->AimingRecoilValue);
		}
		else
		{
			CalcOffset(weapon->m_spreadPower);
		}
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

	FVector2D ammo = FVector2D(m_offset * .5f + 12.f, m_offset * .5f + 12.f);
	Cross_Ammo_Image->SetRenderTranslation(ammo);


	FVector2D up_s = FVector2D(0, -m_offset_s);
	Up_Cross_image_s->SetRenderTranslation(up_s);

	FVector2D down_s = FVector2D(0, m_offset_s);
	Down_Cross_image_s->SetRenderTranslation(down_s);

	FVector2D left_s = FVector2D(-m_offset_s, 0);
	Left_Cross_image_s->SetRenderTranslation(left_s);

	FVector2D right_s = FVector2D(m_offset_s, 0);
	Right_Cross_image_s->SetRenderTranslation(right_s);
}

void UCrosshair_Widget::CalcOffset(float spreadpower)
{
	m_offset = FMath::Lerp(0.0f, spreadpower * 300.0f, m_alpha);
	m_offset_s = FMath::Lerp(0.0f, spreadpower * 150.0f, m_alpha);
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(m_offset));
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(m_alpha));
}

bool UCrosshair_Widget::CalcAlphaValue(float DeltaTime)
{
	if (weapon->bRecoil)
	{
		m_alpha += DeltaTime;// *0.6f;
	}
	else
	{
		if (weapon->bRecovery)
		{
			m_alpha -= DeltaTime * 2.0f;
		}
		else
		{
			m_alpha -= DeltaTime * 0.8f;
		}
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
}

void UCrosshair_Widget::MoveDot()
{
	if (weapon->m_result.bBlockingHit)
	{
		GetWorld()->GetTimerManager().ClearTimer(DotTimer);
		weapon->m_result.Location;
		FVector2D loc;
		FIntVector2 screensize;
		UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), weapon->m_result.Location, loc);
		GetOwningPlayer()->GetViewportSize(screensize.X, screensize.Y);
		loc.X -= screensize.X / 2;
		loc.Y -= screensize.Y / 2;
		Dot_image->SetRenderTranslation(loc);
		//weapon->m_result
		GetWorld()->GetTimerManager().SetTimer(DotTimer,
			FTimerDelegate::CreateLambda([&]()
				{
					Dot_image->SetRenderTranslation(FVector2D(.0f, .0f));

				}
		), .3f, false);
		
	}
}

void UCrosshair_Widget::CheckDie()
{

	APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (this)
	{
		if (weapon->bHit)
		{
			if (UWeaponComponent::CheckActorTag(weapon->m_result.GetActor(), FName("Missile")))
			{
				weapon->bHit = false;
				UGameplayStatics::PlaySoundAtLocation(this, MyCharacter->CharacterSound->missile_Hit_cue, MyCharacter->GetActorLocation());
				Hit_Image_NW->SetBrushTintColor(FSlateColor(FColor::White));
				Hit_Image_NE->SetBrushTintColor(FSlateColor(FColor::White));
				Hit_Image_SW->SetBrushTintColor(FSlateColor(FColor::White));
				Hit_Image_SE->SetBrushTintColor(FSlateColor(FColor::White));
			}
			else
			{
				if (weapon->headhit)
				{
					weapon->bHit = false;
					weapon->headhit = false;
					UGameplayStatics::PlaySoundAtLocation(this, MyCharacter->CharacterSound->Head_Hit_cue, MyCharacter->GetActorLocation());
					Hit_Image_NW->SetBrushTintColor(FSlateColor(FColor::Red));
					Hit_Image_NE->SetBrushTintColor(FSlateColor(FColor::Red));
					Hit_Image_SW->SetBrushTintColor(FSlateColor(FColor::Red));
					Hit_Image_SE->SetBrushTintColor(FSlateColor(FColor::Red));
				}
				else
				{
					weapon->bHit = false;
					UGameplayStatics::PlaySoundAtLocation(this, MyCharacter->CharacterSound->Normal_Hit_cue, MyCharacter->GetActorLocation());
					Hit_Image_NW->SetBrushTintColor(FSlateColor(FColor::White));
					Hit_Image_NE->SetBrushTintColor(FSlateColor(FColor::White));
					Hit_Image_SW->SetBrushTintColor(FSlateColor(FColor::White));
					Hit_Image_SE->SetBrushTintColor(FSlateColor(FColor::White));
				}
			}

			GetWorld()->GetTimerManager().ClearTimer(HitTimer);
			if (IsAnimationPlaying(HitAnim))
			{
				//Hit_Overlay->SetRenderOpacity(0.0f);
				StopAnimation(HitAnim);
			}

			if (HitAnim)
			{
				GetWorld()->GetTimerManager().SetTimer(HitTimer,
					FTimerDelegate::CreateLambda([&]()
						{
							if (HitAnim)
							{
								PlayAnimationForward(HitAnim);
							}
						}
				), .01f, false);
			}

		}

	}
	if (!weapon->m_result.GetActor())
	{
		return;
	}
	UStatComponent* stat = weapon->m_result.GetActor()->FindComponentByClass<UStatComponent>();
	if (stat)
	{
		if (stat->bDie)
		{
			if (UWeaponComponent::CheckActorTag(weapon->m_result.GetActor(), FName("Missile")))
			{
				UGameplayStatics::PlaySoundAtLocation(this, MyCharacter->CharacterSound->missile_Kill_cue, MyCharacter->GetActorLocation());
			}
			else
			{
				UGameplayStatics::PlaySoundAtLocation(this, MyCharacter->CharacterSound->Kill_cue, MyCharacter->GetActorLocation());
			}
			//stat->isDie = false;
			Kill_Overlay->SetRenderOpacity(1.0f);
			GetWorld()->GetTimerManager().ClearTimer(KillTimer);
			if (IsAnimationPlaying(KillAnim))
			{
				StopAnimation(KillAnim);
			}

			GetWorld()->GetTimerManager().SetTimer(KillTimer,
				FTimerDelegate::CreateLambda([&]()
					{
						PlayAnimationForward(KillAnim);
					}
			), .01f, false);
		}
	}
}

void UCrosshair_Widget::SetAmmoImage()
{
	float ammovalue = float(weapon->curAmmo) / float(weapon->maxAmmo);
	Cross_Ammo_Image->SetRenderOpacity(1.f);

	if (weapon->curAmmo <= 10)
	{
		LowAmmoText->SetRenderOpacity(1.f);
		if (!IsAnimationPlaying(LowAmmoAnim))
		{
			PlayAnimation(LowAmmoAnim, 0.f, 0);
		}
	}
	else
	{
		if (IsAnimationPlaying(LowAmmoAnim))
		{
			StopAnimation(LowAmmoAnim);
		}
		LowAmmoText->SetRenderOpacity(0.0f);
	}
	/*switch (weapon->weapontype)
	{
	case EWeaponType::TE_Pistol:
		ammovalue = weapon->curAmmo / 10.0f;
		break;
	case EWeaponType::TE_Rifle:
		ammovalue = weapon->curAmmo / weapon->maxAmmo;
		break;
	case EWeaponType::TE_Shotgun:
		ammovalue = weapon->curAmmo / 7.0f;
		break;
	default:
		ammovalue = weapon->curAmmo / 30.0f;
		break;
	}*/

	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(ammovalue));
	AmmoMat->SetScalarParameterValue(FName(TEXT("Percent")), ammovalue);
	Cross_Ammo_Image->SetBrushFromMaterial(AmmoMat);
}

void UCrosshair_Widget::PlayReloadAnim()
{
	if (this)
	{
		if (IsAnimationPlaying(FadeOutAnim))
		{
			StopAnimation(FadeOutAnim);
		}

		if (IsAnimationPlaying(LowAmmoAnim))
		{
			StopAnimation(LowAmmoAnim);
			LowAmmoText->SetRenderOpacity(0.0f);
		}

		LowAmmoText->SetRenderOpacity(0.0f);
		//Reload_Overlay->SetRenderOpacity(1.0f);
		NotFire_Overlay->SetRenderOpacity(0.0f);
		Crosshair_Overlay->SetRenderOpacity(0.0f);
		Cross_Ammo_Image->SetRenderOpacity(1.0f);


		if (ReloadAnim)
		{
			GetWorld()->GetTimerManager().SetTimer(ReloadTimer, FTimerDelegate::CreateLambda([&]()
				{
					if (ReloadAnim)
					{
						PlayAnimationForward(ReloadAnim);
					}
				}), 0.01f, false);
		}
	}
}

void UCrosshair_Widget::StopReloadAnim()
{
	if (IsAnimationPlaying(FadeOutAnim))
	{
		StopAnimation(FadeOutAnim);
	}

	if (IsAnimationPlaying(ReloadAnim))
	{
		StopAnimation(ReloadAnim);
	}

	Reload_Overlay->SetRenderOpacity(0.0f);

	Cross_Ammo_Image->SetRenderOpacity(1.0f);
	Crosshair_Overlay->SetRenderOpacity(1.0f);

	if (bBlocking)
	{
		Blocking(true);
	}
}

void UCrosshair_Widget::Blocking(bool bBlock)
{
	if (NotFire_Overlay)
	{
		if (IsAnimationPlaying(ReloadAnim))
		{
			return;
		}

		if (bBlock)
		{
			bBlocking = true;
		
			StopAnimation(LowAmmoAnim);
			LowAmmoText->SetRenderOpacity(0.0f);

			Crosshair_Overlay->SetRenderOpacity(.0f);
			Reload_Overlay->SetRenderOpacity(.0f);
			Cross_Ammo_Image->SetRenderOpacity(.0f);
			NotFire_Overlay->SetRenderOpacity(1.f);
		}
		else
		{
			bBlocking = false;

			if (weapon->curAmmo <= 10)
			{
				LowAmmoText->SetRenderOpacity(1.0f);
				if (!IsAnimationPlaying(LowAmmoAnim))
				{
					PlayAnimation(LowAmmoAnim, 0.f, 0);
				}
			}
			Crosshair_Overlay->SetRenderOpacity(1.0f);
			//Reload_Overlay->SetRenderOpacity(1.0f);
			Cross_Ammo_Image->SetRenderOpacity(1.0f);
			NotFire_Overlay->SetRenderOpacity(.0f);
		}
	}
}

void UCrosshair_Widget::SetCombatWidgetVisible()
{
	/*if (this)
	{
		GetWorld()->GetTimerManager().ClearTimer(VisibleTimer);
		
		if (IsAnimationPlaying(FadeOutAnim))
		{
			StopAnimation(FadeOutAnim);
		}
		Crosshair_Overlay->SetRenderOpacity(1.0f);
		Reload_Overlay->SetRenderOpacity(1.0f);
		if (weapon)
		{
			if (weapon->bAiming || weapon->bFire)
			{
				bWidgetVisible = true;
				GetWorld()->GetTimerManager().ClearTimer(VisibleTimer);
				Crosshair_Overlay->SetRenderOpacity(1.0f);
			}
			else if (!weapon->bAiming && !weapon->bFire)
			{
				if (FadeOutAnim)
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
	}*/
}

