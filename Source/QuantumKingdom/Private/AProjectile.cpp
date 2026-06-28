// Fill out your copyright notice in the Description page of Project Settings.


#include "AProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
AAProjectile::AAProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SPhere collision
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;
	SphereCollision->SetCollisionProfileName(TEXT("Projectile"));


	//Static mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	//Movimiento de projectile
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = SphereCollision;
	ProjectileMovement->bShouldBounce = false;

	ProjectileMovement->InitialSpeed = 500.f;      // velocidad inicial
	ProjectileMovement->MaxSpeed = 3000.f;           // velocidad máxima
	ProjectileMovement->ProjectileGravityScale = 0.f; // 0 = sin gravedad, 1 = gravedad normal

}

// Called when the game starts or when spawned
void AAProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(
		this, &AAProjectile::OnSphereBeginOverlap
	);

	ProjectileMovement->OnProjectileStop.AddDynamic(
		this, &AAProjectile::OnProjectileStop
	);

}


void AAProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || !OtherComp) return;

	if (OtherComp->IsSimulatingPhysics())
	{
		FVector Impulse = ProjectileMovement->Velocity * 1.0f;
		OtherComp->AddImpulse(Impulse, NAME_None, false);
	}

	// ── Pasa también OtherComp ──────────────────
	HandleTagActions(OtherActor, OtherComp);
}

void AAProjectile::OnProjectileStop(const FHitResult& ImpactResult)
{
	// ── IMAGE 3: Disable Collision ────────────────────────────────
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ── IMAGE 2: Attach al componente golpeado ────────────────────
	UPrimitiveComponent* HitComp = ImpactResult.GetComponent();
	if (HitComp)
	{
		FAttachmentTransformRules AttachRules(
			EAttachmentRule::KeepWorld,  // Location Rule
			EAttachmentRule::KeepWorld,  // Rotation Rule
			EAttachmentRule::KeepWorld,  // Scale Rule
			/*bWeldSimulatedBodies=*/true
		);
		AttachToComponent(HitComp, AttachRules);
	}

	// ── IMAGE 2: Set Actor Location al Impact Point ───────────────
	if (ImpactResult.bBlockingHit || ImpactResult.bStartPenetrating)
	{
		SetActorLocation(
			ImpactResult.ImpactPoint,
			/*bSweep=*/false,
			nullptr,
			ETeleportType::None
		);
	}
}


void AAProjectile::HandleTagActions(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	if (!OtherActor || !OtherComp) return;

	// Debug — muestra el nombre del componente golpeado
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
			FString::Printf(TEXT("Hit component: %s"), *OtherComp->GetName()));
	}

	// Comprueba tag en el actor Y en el componente
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
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Settings hit"));
		return;
	}

	if (bHasExit)
	{
		UKismetSystemLibrary::QuitGame(
			this,
			UGameplayStatics::GetPlayerController(this, 0),
			EQuitPreference::Quit,
			false
		);
	}
}
