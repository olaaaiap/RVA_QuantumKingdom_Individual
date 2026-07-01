// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Sound/SoundBase.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/InputSettings.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include "AProjectile.h"
#include "APistol.generated.h"

UCLASS()
class QUANTUMKINGDOM_API AAPistol : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPistol();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SM_Pistol;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* AmmoWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* MuzzleLocation;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
	UHapticFeedbackEffect_Base* PistolFireHapticEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Shoot_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Shoot_Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* IMC_Weapon_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* IMC_Weapon_Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<AAProjectile> ProjectileClass;

	//Funciones para cuando el arma se agarra / se suelta
	UFUNCTION(BlueprintCallable, Category = "Grab")
	void OnWeaponGrabbed();

	UFUNCTION(BlueprintCallable, Category = "Grab")
	void OnWeaponDropped();

	//Disparar
	void OnShootLeft();
	void OnShootRight();

	//Variable para saber si se está agarrando el arma. si es asi, no se disparará
	UPROPERTY()
	bool bIsHeld = false;

public:	
	virtual void TryShoot(bool bIsLeftHand);
	void SpawnProjectile(bool bIsLeftHand);
	void UpdateAmmoWidget();
	void AddWeaponInputContext();
	void RemoveWeaponInputContext();

private:
	UPROPERTY()
	UUserWidget* CachedWidget;
};
