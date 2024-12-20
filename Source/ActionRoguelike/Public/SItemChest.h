// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()	
	
public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	bool PlayLidAnim;
	float LidAnimDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool ShouldOpenLid;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere)
	float OpenLidPitch;

	UPROPERTY(EditAnywhere)
	float CloseLidPitch;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interact_Implementation(APawn* InstigatorPawn);

};
