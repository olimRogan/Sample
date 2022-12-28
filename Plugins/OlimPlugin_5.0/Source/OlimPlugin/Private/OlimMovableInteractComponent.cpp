// Fill out your copyright notice in the Description page of Project Settings.

#include "OlimMovableInteractComponent.h"
#include "OlimInteractActor.h"
#include "Kismet/KismetMathLibrary.h"

UOlimMovableInteractComponent::UOlimMovableInteractComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}

void UOlimMovableInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UOlimMovableInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InteractTimeline.TickTimeline(DeltaTime);

	if(ComponentState == EOlimMovableComponentState::EOMCS_Moving)
	{
		if(InteractActor)
		{
			Interaction(CurrentProperty,InteractActor.Get()->MeshComponent);	
		}
	}
}

void UOlimMovableInteractComponent::GetProperty(const FString& name)
{
	if(ComponentState == EOlimMovableComponentState::EOMCS_Wait)
	{
		// Key : FString, Value : FMovableProperty
		for (TTuple<FString, FOlimMovableProperty>& Item : MovableList)
		{
			if(name.Equals(Item.Key))
			{
				CurrentProperty = Item.Value;
				PlayTimeline(CurrentProperty.GetValue().Curve);
			}
		}
	}
}

void UOlimMovableInteractComponent::Interaction(const TOptional<FOlimMovableProperty>& property,
	TObjectPtr<UStaticMeshComponent> mesh)
{
	if(!property.IsSet() || mesh == nullptr) return;

	const FOlimTransform toTransform = property.GetValue().ToTransform;
	const FOlimTransform fromTransform = property.GetValue().FromTransform;
	
	const FVector aLoc = UKismetMathLibrary::VLerp(fromTransform.Location,toTransform.Location,TimelineAlpha);
	const FVector bLoc = UKismetMathLibrary::VLerp(toTransform.Location,fromTransform.Location,TimelineAlpha);

	const FRotator aRot = UKismetMathLibrary::RLerp(fromTransform.Rotation,toTransform.Rotation,TimelineAlpha,false);
	const FRotator bRot = UKismetMathLibrary::RLerp(toTransform.Rotation,fromTransform.Rotation,TimelineAlpha,false);
	
	const FVector newLocation = UKismetMathLibrary::SelectVector(aLoc,bLoc,!property.GetValue().bReverse);
	const FRotator newRotation = UKismetMathLibrary::SelectRotator(aRot,bRot,!property.GetValue().bReverse);
	
	mesh->SetRelativeLocation(newLocation);
	mesh->SetRelativeRotation(newRotation);
}

void UOlimMovableInteractComponent::Interact(const FString& string, EOlimActorType type)
{
	// TODO CPP
}

void UOlimMovableInteractComponent::InteractBP_Implementation(const FString& str,const EOlimActorType type)
{
	switch (type)
	{
	case EOlimActorType::EAS_Actor:
		if(ComponentState == EOlimMovableComponentState::EOMCS_Wait)
		{
			if(str.Equals("Open")) {MovementState = EOlimMovementState::EOMS_Open;}
			else if(str.Equals("Close")) {MovementState = EOlimMovementState::EOMS_Close;}	
		}
		GetProperty(str);
		break;
	case EOlimActorType::EAS_ActorBucket:
		break;
	case EOlimActorType::EAS_EventHandler:
		break;
	case EOlimActorType::EAS_DefaultMAX:
		break;
	default: ;
	}
}

void UOlimMovableInteractComponent::PlayTimeline(UCurveFloat* curve)
{
	if(curve != nullptr)
	{
		FOnTimelineFloat TimelineFloat;
		FOnTimelineEvent FinishEvent;
		TimelineFloat.BindUFunction(this,FName("TimelineProgress"));
		FinishEvent.BindUFunction(this,FName("TimelineFinish"));

		InteractTimeline.AddInterpFloat(curve,TimelineFloat);
		InteractTimeline.SetTimelineFinishedFunc(FinishEvent);
		
		ComponentState = EOlimMovableComponentState::EOMCS_Moving;
		InteractTimeline.PlayFromStart();
	}
}

void UOlimMovableInteractComponent::TimelineProgress(float value)
{
	TimelineAlpha = value;
}

void UOlimMovableInteractComponent::TimelineFinish()
{
	ComponentState = EOlimMovableComponentState::EOMCS_Wait;
}

