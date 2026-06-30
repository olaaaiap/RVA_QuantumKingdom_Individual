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
	// Sets default values for this actor's properties
	AAPistol();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//Componentes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SM_Pistol;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* AmmoWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* MuzzleLocation;


	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
	UHapticFeedbackEffect_Base* PistolFireHapticEffect;

	//Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Shoot_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Shoot_Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* IMC_Weapon_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* IMC_Weapon_Right;

	//Projectile
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<AAProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable, Category = "Grab")
	void OnWeaponGrabbed();

	UFUNCTION(BlueprintCallable, Category = "Grab")
	void OnWeaponDropped();

	//Disparo
	void OnShootLeft();
	void OnShootRight();

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
