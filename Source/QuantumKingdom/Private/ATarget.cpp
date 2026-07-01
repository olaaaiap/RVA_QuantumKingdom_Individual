// Fill out your copyright notice in the Description page of Project Settings.


#include "ATarget.h"
#include "Components/BoxComponent.h"
#include <ALevel2Manager.h>
#include <AProjectile.h>
#include <Kismet/GameplayStatics.h>

AATarget::AATarget()
{
	PrimaryActorTick.bCanEverTick = true;

    USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    RootComponent = SceneRoot;

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    Box->SetupAttachment(RootComponent);
    Box->SetRelativeScale3D(FVector(3.73f, 1.06f, 9.79f));

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(RootComponent);

    
}

void AATarget::BeginPlay()
{
	Super::BeginPlay();

    DirecNorm = Direction.GetSafeNormal(0.0001f);
    Origin = GetActorLocation();

    OnActorBeginOverlap.AddDynamic(this, &AATarget::HandleOverlap);
}

void AATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (!IsMobile) return;
    TimeCount += DeltaTime;
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

    return OtherActor->IsA<AAProjectile>();
}

void AATarget::OnCollide()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(),AALevel2Manager::StaticClass(),FoundActors);

    if (FoundActors.Num() > 0)
    {
        AALevel2Manager* Manager = Cast<AALevel2Manager>(FoundActors[0]);
        if (Manager)
        {
            Manager->AddEnemyDead();
        }
    }

    Destroy();
}
