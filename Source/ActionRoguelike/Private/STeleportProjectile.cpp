// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASTeleportProjectile::ASTeleportProjectile()
	: ASBaseProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComp->SetCollisionProfileName("Projectile");
	MovementComp->InitialSpeed = 1000.0f;
	SphereComp->OnComponentHit.AddDynamic(this, &ASTeleportProjectile::OnHit);
}

// Called every frame
void ASTeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	float LifeTime = 0.3f;
	GetWorldTimerManager().SetTimer(TimerHandle_Explosion, this, &ASTeleportProjectile::LifeTimeElapsed, LifeTime);
}

void ASTeleportProjectile::LifeTimeElapsed()
{
	if (ensure(ExplosionEffect))
	{
		EffectComp->Deactivate();
		MovementComp->StopMovementImmediately();
		FTransform SpawnTM = FTransform(GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, SpawnTM);
	}

	float LifeTime = 0.2f;
	GetWorldTimerManager().SetTimer(TimerHandle_TeleportPlayer, this, &ASTeleportProjectile::TeleportPlayer, LifeTime);
}

void ASTeleportProjectile::TeleportPlayer()
{
	AActor* Player = GetInstigator();
	if (Player != nullptr)
	{
		Player->SetActorLocation(GetActorLocation());
		UE_LOG(LogTemp, Log, TEXT("Teleported actor %s"), *Player->GetActorNameOrLabel());
	}

	Destroy();
}

void ASTeleportProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
						UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != GetInstigator())
	{
		LifeTimeElapsed();
	}
}
