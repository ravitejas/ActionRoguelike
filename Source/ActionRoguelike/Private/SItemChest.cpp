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
	CloseLidPitch = 1;
	PlayLidAnim = false;
	LidAnimDuration = 1.0f;
	ShouldOpenLid = false;
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	PlayLidAnim = true;
	ShouldOpenLid = !ShouldOpenLid;
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

	if (PlayLidAnim)
	{
		FRotator const& LidRelRot = LidMesh->GetRelativeRotation();
		float const DeltaLidAngle = (OpenLidPitch * DeltaTime) / LidAnimDuration;
		float const NextPitch = LidRelRot.Pitch + DeltaLidAngle * (ShouldOpenLid ? 1.0f : -1.0f);
		LidMesh->SetRelativeRotation(FRotator(NextPitch, 0, 0));

// 		if (GEngine)
// 		{
// 			GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, 
// 				FString::Printf(TEXT("Cur Pitch: %.3f, Next Pitch: %.3f"), LidRelRot.Pitch, NextPitch));
// 		}

		if (ShouldOpenLid)
		{
			if (NextPitch > OpenLidPitch)
			{
				// finished opening the lid
				PlayLidAnim = false;
			}
		}
		else
		{
			if (NextPitch < CloseLidPitch)
			{
				// finished closing the lid
				PlayLidAnim = false;
			}
		}
	}
}

