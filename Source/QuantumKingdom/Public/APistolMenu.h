// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "APistol.h"
#include "APistolMenu.generated.h"

/**
 * 
 */
UCLASS()
class QUANTUMKINGDOM_API AAPistolMenu : public AAPistol
{
	GENERATED_BODY()
	
protected:
	//Sobreescribir funcion para que no se controle la cantidad de ammo. Esta pistola tiene ammo ilimitado
	virtual void TryShoot(bool bIsLeftHand) override;

};
