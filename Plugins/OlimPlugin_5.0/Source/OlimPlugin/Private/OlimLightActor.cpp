// Fill out your copyright notice in the Description page of Project Settings.


#include "OlimLightActor.h"

#include "OlimInteractActor.h"
#include "OlimLightInteractComponent.h"
#include "OlimMovableInteractComponent.h"

// Sets default values
AOlimLightActor::AOlimLightActor()
{
	
}

// Called when the game starts or when spawned
void AOlimLightActor::BeginPlay()
{
	Super::BeginPlay();

	for (const AOlimInteractActor* Actor : InteractActors)
	{
		if(Actor)
		{
			Actor->GetLightInteractComponent()->LightActor = this;	
		}
	}
}

void AOlimLightActor::Interact(const FString& string, EOlimActorType type)
{
	ActiveActors.Empty();
	if(!InteractActors.IsEmpty())
	{
		for (AOlimInteractActor* Actor : InteractActors)
		{
			if(Actor)
			{
				if(Actor->GetMovableInteractComponent().Get()->MovementState == EOlimMovementState::EOMS_Open)
				{
					ActiveActors.Emplace(Actor);
				}
			}
		}
		if(ActiveActors.Num() > 1)
		{
			bCanTurnOn = false;
		}
		else
		{
			if(ActiveActors.Num() == 0)
			{
				bCanTurnOn = true;
			}
		}
	}
}

