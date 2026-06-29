// Fill out your copyright notice in the Description page of Project Settings.


#include "ADamageSphere.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AADamageSphere::AADamageSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// DefaultSceneRoot
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = Root;

	// P_Fire — sistema de partículas del fuego
	P_Fire = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("P_Fire"));
	P_Fire->SetupAttachment(RootComponent);

	// En_Campfire_01 — mesh de la hoguera
	En_Campfire_01 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("En_Campfire_01"));
	En_Campfire_01->SetupAttachment(RootComponent);

	// Sphere — collider de daño
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

// Called when the game starts or when spawned
void AADamageSphere::BeginPlay()
{
	Super::BeginPlay();
	
	/*Sphere->OnComponentBeginOverlap.AddDynamic(
		this, &AADamageSphere::OnSphereBeginOverlap
	);*/

	// Busca el Sphere por nombre en runtime
	TArray<USphereComponent*> Spheres;
	GetComponents<USphereComponent>(Spheres);

	if (Spheres.Num() > 0)
	{
		Sphere = Spheres[0];
		Sphere->OnComponentBeginOverlap.AddDynamic(
			this, &AADamageSphere::OnSphereBeginOverlap
		);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Sphere encontrado OK"));
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("ERROR: Sphere no encontrado"));
	}
}

//void AADamageSphere::OnSphereBeginOverlap(
//	UPrimitiveComponent* OverlappedComponent,
//	AActor* OtherActor,
//	UPrimitiveComponent* OtherComp,
//	int32                OtherBodyIndex,
//	bool                 bFromSweep,
//	const FHitResult& SweepResult)
//{
//	if (!OtherActor) return;
//
//	// Equivale a: Cast To OlaiaVRPawn
//	// Comprueba que el actor que entra es el VR Pawn del jugador
//	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
//	if (OtherActor != PlayerPawn) return;
//
//	// Equivale a: Take Damage (función custom del VR Pawn BP sin parámetros)
//	FName FuncName = FName("Take Damage");
//	if (UFunction* Func = OtherActor->FindFunction(FuncName))
//	{
//		OtherActor->ProcessEvent(Func, nullptr);
//	}
//}

void AADamageSphere::OnSphereBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	
	if (OtherActor != PlayerPawn) return;

	FName FuncName = FName("TakeDamage");
	UFunction* Func = OtherActor->FindFunction(FuncName);

	
	if (Func)
		OtherActor->ProcessEvent(Func, nullptr);
}


