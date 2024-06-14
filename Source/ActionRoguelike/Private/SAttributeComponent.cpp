// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = 100;
}

bool USAttributeComponent::AddHealth(float healthDelta)
{
	if (ensureAlways(healthDelta < 0) && Health > 0)
	{
		Health += healthDelta;
		OnHealthChanged.Broadcast(nullptr, this, Health, healthDelta);
		if (Health < 0)
		{
			Health = 0;
		}

		return true;
	}

	return false;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

