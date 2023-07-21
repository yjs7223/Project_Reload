// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"
#include "PlayerCharacter.h"
#include "PlayerWeaponComponent.h"
#include "Player_HP_Widget.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "UMG.h"

void UPlayerHUDWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	owner = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	SetVisibility(ESlateVisibility::Visible);
}

void UPlayerHUDWidget::NativeConstruct()
{

}

void UPlayerHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPlayerHUDWidget::TranslateHPWidget()
{
	FWidgetTransform wt;
	FTransform tf = owner->GetMesh()->GetSocketTransform(TEXT("HP_Widget_Socket"));
	UGameplayStatics::ProjectWorldToScreen(owner->GetLocalViewingPlayerController(), tf.GetLocation(), wt.Translation);
	wt.Scale = FVector2D(1, 1);
	WBP_PlayerHP->SetRenderTransform(wt);
	WBP_PlayerHP;
	//GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, wt.Translation.ToString());
}

void UPlayerHUDWidget::TranslateAmmoWidget()
{
	FVector socketloc = owner->FindComponentByClass<UPlayerWeaponComponent>()->WeaponMesh->GetSocketLocation(TEXT("AmmoWidgetSocket"));
	FVector2D wloc;
	UGameplayStatics::ProjectWorldToScreen(owner->GetLocalViewingPlayerController(), socketloc, wloc);
	WBP_AMMO->SetRenderTranslation(wloc);
}

