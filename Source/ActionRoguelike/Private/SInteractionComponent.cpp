// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	AActor* MyOwner = GetOwner();

	FVector EyeLoc;
	FRotator EyeRot;
	MyOwner->GetActorEyesViewPoint(EyeLoc, EyeRot);
	FVector EndLoc = EyeLoc + (EyeRot.Vector() * 300);

	// FHitResult HitResult;	
	// bool IsBlockingHit = GetWorld()->LineTraceSingleByObjectType(HitResult, EyeLoc, EndLoc, QueryParams);

	TArray< FHitResult> HitResults;
	FCollisionShape SphereColl;
	float Radius = 30.f;
	SphereColl.SetSphere(Radius);
	bool IsBlockingHit = GetWorld()->SweepMultiByObjectType(HitResults, EyeLoc, EndLoc, FQuat::Identity, QueryParams, SphereColl);
	FColor LineColor = IsBlockingHit ? FColor::Green : FColor::Red;

	for (FHitResult HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor != nullptr && HitActor->Implements<USGameplayInterface>())
		{
			DrawDebugSphere(GetWorld(), HitResult.Location, Radius, 32, LineColor, false, 2.0f);
			APawn* MyPawn = Cast<APawn>(MyOwner);
			ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
			break;
		}
	}
	
	DrawDebugLine(GetWorld(), EyeLoc, EndLoc, LineColor, false, 2.0f, 0, 2.0f);
}
