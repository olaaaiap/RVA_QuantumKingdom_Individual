// Fill out your copyright notice in the Description page of Project Settings.


#include "APistol.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AAPistol::AAPistol()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SM_Pistol como root
	SM_Pistol = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Pistol"));
	RootComponent = SM_Pistol;

	// MuzzleLocation — punto de spawn del proyectil
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(SM_Pistol);

	// AmmoWidget — widget 3D en el mundo
	AmmoWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("AmmoWidget"));
	AmmoWidget->SetupAttachment(SM_Pistol);

	// GrabComponentSnap se gestiona desde el BP, no se crea aquí

	// Valores por defecto
	CurrentAmmo = 10;
	MaxAmmo = 10;
}

// Called when the game starts or when spawned
void AAPistol::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAPistol::OnWeaponGrabbed()
{
	AddWeaponInputContext();
}

void AAPistol::OnWeaponDropped()
{
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

	
	
	if (CurrentAmmo <= 0) {
		return;
	}

	SpawnProjectile(bIsLeftHand);
}

void AAPistol::SpawnProjectile(bool bIsLeftHand)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
		FString::Printf(TEXT("ProjectileClass: %s"), ProjectileClass ? TEXT("OK") : TEXT("NULL")));
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
		FString::Printf(TEXT("MuzzleLocation: %s"), MuzzleLocation ? TEXT("OK") : TEXT("NULL")));

	if (!ProjectileClass || !MuzzleLocation) return;
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("CON AMMO"));

	UWorld* World = GetWorld();
	if (!World) return;

	// Spawn en MuzzleLocation
	FTransform SpawnTransform(
		MuzzleLocation->GetComponentRotation(),
		MuzzleLocation->GetComponentLocation()
	);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	World->SpawnActor<AAProjectile>(ProjectileClass, SpawnTransform, SpawnParams);

	// Restar munición
	CurrentAmmo = FMath::Max(0, CurrentAmmo - 1);

	// Actualizar widget
	UpdateAmmoWidget();

	// Reproducir sonido
	if (FireSound)
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());

	// Haptic feedback en la mano correcta
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC && PistolFireHapticEffect)
	{
		EControllerHand Hand = bIsLeftHand ? EControllerHand::Left : EControllerHand::Right;
		PC->PlayHapticEffect(PistolFireHapticEffect, Hand, 1.0f, false);
	}

	// Debug
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan,
			FString::Printf(TEXT("Ammo: %d / %d"), CurrentAmmo, MaxAmmo));
	}
}

void AAPistol::UpdateAmmoWidget()
{
	if (!AmmoWidget) return;

	UUserWidget* Widget = AmmoWidget->GetUserWidgetObject();
	if (!Widget) return;

	// Munición actual
	UTextBlock* AmmoText = Cast<UTextBlock>(
		Widget->GetWidgetFromName(FName("TextBlock_AMMO_a"))
	);
	if (AmmoText)
		AmmoText->SetText(FText::AsNumber(CurrentAmmo));

	// Munición máxima
	UTextBlock* MaxAmmoText = Cast<UTextBlock>(
		Widget->GetWidgetFromName(FName("TextBlock_AMMO_st"))
	);
	if (MaxAmmoText)
		MaxAmmoText->SetText(FText::AsNumber(MaxAmmo));
}

void AAPistol::AddWeaponInputContext()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC) return;

	EnableInput(PC);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent))
	{

		if (IA_Shoot_Left)
			EIC->BindAction(IA_Shoot_Left, ETriggerEvent::Started, this, &AAPistol::OnShootLeft);

		if (IA_Shoot_Right)
			EIC->BindAction(IA_Shoot_Right, ETriggerEvent::Started, this, &AAPistol::OnShootRight);
	}

	if (ULocalPlayer* LP = PC->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (IMC_Weapon_Left)
				Subsystem->AddMappingContext(IMC_Weapon_Left, 1);
			if (IMC_Weapon_Right)
				Subsystem->AddMappingContext(IMC_Weapon_Right, 1);
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
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (IMC_Weapon_Left)
				Subsystem->RemoveMappingContext(IMC_Weapon_Left);

			if (IMC_Weapon_Right)
				Subsystem->RemoveMappingContext(IMC_Weapon_Right);
		}
	}
}
