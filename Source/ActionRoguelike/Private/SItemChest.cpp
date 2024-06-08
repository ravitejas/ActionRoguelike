// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	OpenLidPitch = 89;
	PlayLidOpenAnim = false;
	LidOpenAnimDuration = 1.0f;
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	PlayLidOpenAnim = true;
}


// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayLidOpenAnim)
	{
		FRotator const& LidRelRot = LidMesh->GetRelativeRotation();
		float const DeltaLidAngle = (OpenLidPitch * DeltaTime) / LidOpenAnimDuration;
		float const NextPitch = LidRelRot.Pitch + DeltaLidAngle;
		LidMesh->SetRelativeRotation(FRotator(NextPitch, 0, 0));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, 
				FString::Printf(TEXT("Cur Pitch: %.3f, Next Pitch: %.3f"), LidRelRot.Pitch, NextPitch));
		}

		if (NextPitch > OpenLidPitch)
		{
			// finished opening the lid
			PlayLidOpenAnim = false;
		}
	}
}

