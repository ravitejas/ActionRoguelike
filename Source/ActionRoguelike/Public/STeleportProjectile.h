// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseProjectile.h"
#include "Engine/TimerHandle.h"
#include "STeleportProjectile.generated.h"

class UParticleSystem;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASTeleportProjectile : public ASBaseProjectile
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* ExplosionEffect;

	FTimerHandle TimerHandle_Explosion;
	FTimerHandle TimerHandle_TeleportPlayer;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LifeTimeElapsed();
	void TeleportPlayer();
	
public:
	ASTeleportProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
