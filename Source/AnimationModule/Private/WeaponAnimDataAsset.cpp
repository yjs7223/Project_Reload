// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAnimDataAsset.h"

bool FWeaponAnimation::IsVaild()
{
	return Aiming && Aiming_Shooting && Reload && AimOffset;
}
