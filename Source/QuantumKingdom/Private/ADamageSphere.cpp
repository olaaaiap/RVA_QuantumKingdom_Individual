
#include "ADamageSphere.h"
#include "Kismet/GameplayStatics.h"

AADamageSphere::AADamageSphere()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = Root;

	P_Fire = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("P_Fire"));
	P_Fire->SetupAttachment(RootComponent);

	En_Campfire_01 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("En_Campfire_01"));
	En_Campfire_01->SetupAttachment(RootComponent);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AADamageSphere::BeginPlay()
{
	Super::BeginPlay();
	
	if (Sphere)
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &AADamageSphere::OnSphereBeginOverlap);
	}
}

void AADamageSphere::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	
	//Mirar si el player ha chocado con la esfera
	if (OtherActor != PlayerPawn) return;

	//Llamar a la función take damage del player
	UFunction* Func = OtherActor->FindFunction(FName("TakeDamage"));
	if (Func) OtherActor->ProcessEvent(Func, nullptr);
}


