// Fill out your copyright notice in the Description page of Project Settings.


#include "AProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AAProjectile::AAProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;
	SphereCollision->SetCollisionProfileName(TEXT("Projectile"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = SphereCollision;
	ProjectileMovement->bShouldBounce = false;

	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

}

void AAProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AAProjectile::OnSphereBeginOverlap);

	ProjectileMovement->OnProjectileStop.AddDynamic(this, &AAProjectile::OnProjectileStop);

}


void AAProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || !OtherComp) return;

	if (OtherComp->IsSimulatingPhysics())
	{
		FVector Impulse = ProjectileMovement->Velocity * 1.0f;
		OtherComp->AddImpulse(Impulse, NAME_None, false);
	}

	HandleInitialButtons(OtherActor, OtherComp);
}

void AAProjectile::OnProjectileStop(const FHitResult& ImpactResult)
{
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UPrimitiveComponent* HitComp = ImpactResult.GetComponent();
	if (HitComp)
	{
		FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld,EAttachmentRule::KeepWorld,EAttachmentRule::KeepWorld, true);
		AttachToComponent(HitComp, AttachRules);
	}

	if (ImpactResult.bBlockingHit || ImpactResult.bStartPenetrating)
	{
		SetActorLocation(ImpactResult.ImpactPoint,false,nullptr,ETeleportType::None);
	}
}


void AAProjectile::HandleInitialButtons(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	if (!OtherActor || !OtherComp) return;

	bool bHasPlay = OtherActor->ActorHasTag("Play") || OtherComp->ComponentHasTag("Play");
	bool bHasSettings = OtherActor->ActorHasTag("Settings") || OtherComp->ComponentHasTag("Settings");
	bool bHasExit = OtherActor->ActorHasTag("Exit") || OtherComp->ComponentHasTag("Exit");

	if (bHasPlay)
	{
		UGameplayStatics::OpenLevel(this, FName("Ship"));
		return;
	}

	if (bHasSettings)
	{
		return;
	}

	if (bHasExit)
	{
		UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit,	false);
	}
}
