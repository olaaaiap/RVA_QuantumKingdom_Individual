// Fill out your copyright notice in the Description page of Project Settings.


#include "APistolMenu.h"

void AAPistolMenu::TryShoot(bool bIsLeftHand)
{
    SpawnProjectile(bIsLeftHand);
}
