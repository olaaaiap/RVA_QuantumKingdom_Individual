// Fill out your copyright notice in the Description page of Project Settings.


#include "APistol.h"
#include <Kismet/GameplayStatics.h>

AAPistol::AAPistol()
{
	PrimaryActorTick.bCanEverTick = true;

	SM_Pistol = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Pistol"));
	RootComponent = SM_Pistol;

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(SM_Pistol);

	AmmoWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("AmmoWidget"));
	AmmoWidget->SetupAttachment(SM_Pistol);

	CurrentAmmo = 500;
	MaxAmmo = 1000;

}

void AAPistol::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this]()
		{
			if (AmmoWidget) CachedWidget = AmmoWidget->GetUserWidgetObject();
			UpdateAmmoWidget();
		}, 0.5f, false);
}

void AAPistol::OnWeaponGrabbed()
{
	bIsHeld = true;
	AddWeaponInputContext();
}

void AAPistol::OnWeaponDropped()
{
	bIsHeld = false;
	RemoveWeaponInputContext();
}

void AAPistol::OnShootLeft()
{
	TryShoot(true);
}

void AAPistol::OnShootRight()
{
	TryShoot(false);
}

void AAPistol::TryShoot(bool bIsLeftHand)
{
	if (!bIsHeld) return;
	if (CurrentAmmo <= 0) {
		return;
	}

	SpawnProjectile(bIsLeftHand);
}

void AAPistol::SpawnProjectile(bool bIsLeftHand)
{
	if (!ProjectileClass || !MuzzleLocation) return;
	
	UWorld* World = GetWorld();
	if (!World) return;

	FTransform SpawnTransform(MuzzleLocation->GetComponentRotation(),MuzzleLocation->GetComponentLocation());

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	World->SpawnActor<AAProjectile>(ProjectileClass, SpawnTransform, SpawnParams);

	CurrentAmmo = FMath::Max(0, CurrentAmmo - 1);

	UpdateAmmoWidget();

	if (FireSound) UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC && PistolFireHapticEffect)
	{
		EControllerHand Hand = bIsLeftHand ? EControllerHand::Left : EControllerHand::Right;
		PC->PlayHapticEffect(PistolFireHapticEffect, Hand, 1.0f, false);
	}

}


void AAPistol::UpdateAmmoWidget()
{
	
	if (!CachedWidget) return;

	UTextBlock* AmmoText = Cast<UTextBlock>(CachedWidget->GetWidgetFromName(FName("TextBlock_AMMO_available")));
	UTextBlock* MaxAmmoText = Cast<UTextBlock>(CachedWidget->GetWidgetFromName(FName("TextBlock_AMMO_storage")));

	if (AmmoText)
	{
		AmmoText->SetText(FText::AsNumber(CurrentAmmo));
	}

	if (MaxAmmoText) {
		MaxAmmoText->SetText(FText::AsNumber(MaxAmmo));
	}
}

void AAPistol::AddWeaponInputContext()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC) return;

	EnableInput(PC);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent))
	{

		if (IA_Shoot_Left) EIC->BindAction(IA_Shoot_Left, ETriggerEvent::Started, this, &AAPistol::OnShootLeft);

		if (IA_Shoot_Right) EIC->BindAction(IA_Shoot_Right, ETriggerEvent::Started, this, &AAPistol::OnShootRight);
	}

	if (ULocalPlayer* LP = PC->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (IMC_Weapon_Left) Subsystem->AddMappingContext(IMC_Weapon_Left, 1);
			if (IMC_Weapon_Right) Subsystem->AddMappingContext(IMC_Weapon_Right, 1);
		}
	}
}

void AAPistol::RemoveWeaponInputContext()
{

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC) return;

	DisableInput(PC);

	if (ULocalPlayer* LP = PC->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (IMC_Weapon_Left) Subsystem->RemoveMappingContext(IMC_Weapon_Left);

			if (IMC_Weapon_Right) Subsystem->RemoveMappingContext(IMC_Weapon_Right);
		}
	}
}
