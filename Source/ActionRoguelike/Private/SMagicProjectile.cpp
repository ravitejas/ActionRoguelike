// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SAttributeComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
	: ASBaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp->SetCollisionProfileName("Projectile");
	MovementComp->InitialSpeed = 1300.0f;

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnBeginOverlap);
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASMagicProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
	const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		USAttributeComponent* AttrComp = OtherActor->GetComponentByClass<USAttributeComponent>();
		if (AttrComp)
		{
			static float PROJECTILE_DAMAGE = -20.f;
			AttrComp->AddHealth(PROJECTILE_DAMAGE);
			Destroy();
		}
	}
}
