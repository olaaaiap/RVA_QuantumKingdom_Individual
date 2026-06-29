// Fill out your copyright notice in the Description page of Project Settings.


#include "ATarget.h"
#include "Components/BoxComponent.h"
#include <ALevel2Manager.h>
#include <AProjectile.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AATarget::AATarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Crear una raíz neutra
    USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    RootComponent = SceneRoot;

    // Box y StaticMesh cuelgan de la raíz, independientes entre sí
    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    Box->SetupAttachment(RootComponent);
    Box->SetRelativeScale3D(FVector(3.73f, 1.06f, 9.79f));

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(RootComponent);

    
}

// Called when the game starts or when spawned
void AATarget::BeginPlay()
{
	Super::BeginPlay();

    // Normalizar Direction y guardar Origin (igual que en el BP)
    DirecNorm = Direction.GetSafeNormal(0.0001f);
    Origin = GetActorLocation();

    // Registrar el overlap
    OnActorBeginOverlap.AddDynamic(this, &AATarget::HandleOverlap);

}

// Called every frame
void AATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (!IsMobile) return;

    TimeCount += DeltaTime;

    // Movimiento sinusoidal: Origin + DirecNorm * sin(TimeCount * Speed) * Amplitude
    float SinValue = FMath::Sin(TimeCount * Speed) * Amplitude;
    FVector NewLocation = Origin + DirecNorm * SinValue;

    SetActorLocation(NewLocation);
}

void AATarget::HandleOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (CanCollide(OtherActor))
    {
        OnCollide();
    }
}

bool AATarget::CanCollide(AActor* OtherActor)
{
    if (!OtherActor) return false;

    // Devuelve true solo si el actor que colisiona es un Projectile
    return OtherActor->IsA<AAProjectile>();
}

void AATarget::OnCollide()
{
    // Buscar el Level2Manager en escena y notificarle
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        AALevel2Manager::StaticClass(),
        FoundActors
    );

    if (FoundActors.Num() > 0)
    {
        AALevel2Manager* Manager = Cast<AALevel2Manager>(FoundActors[0]);
        if (Manager)
        {
            Manager->AddEnemyDead(); // ajusta si el nombre exacto es diferente
        }
    }

    Destroy();
}
