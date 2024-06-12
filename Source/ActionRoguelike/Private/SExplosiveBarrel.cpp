// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetCollisionProfileName("PhysicsActor");
	RootComponent = StaticMeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetAutoActivate(false);
	RadialForceComp->Radius = 650;
	RadialForceComp->ImpulseStrength = 2000;
	RadialForceComp->ForceStrength = 10;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void ASExplosiveBarrel::Explode()
{
	RadialForceComp->FireImpulse();
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	StaticMeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnBarrelComponentHit);
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

// 	FScriptDelegate hitDelegate;
// 	hitDelegate.BindUFunction(this, "OnBarrelComponentHit");
// 	StaticMeshComp->OnComponentHit.Add(hitDelegate);
	
}

void ASExplosiveBarrel::OnBarrelComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();

	FString InfoMessage = FString::Printf(TEXT("Hit at loc: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, InfoMessage, nullptr, FColor::Green, 2.0f, true);
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

