// Fill out your copyright notice in the Description page of Project Settings.


#include "OlimMovableInteractComponent.h"

#include "OlimInteractActor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UOlimMovableInteractComponent::UOlimMovableInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UOlimMovableInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
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

void UOlimMovableInteractComponent::Interaction(TOptional<FOlimMovableProperty>& property,
	TObjectPtr<UStaticMeshComponent> mesh)
{
	if(!property.IsSet() || mesh == nullptr) return;

	const FTransform toTransform = property.GetValue().ToTransform;
	const FTransform fromTransform = property.GetValue().FromTransform;

	const FTransform a = UKismetMathLibrary::TLerp(fromTransform,toTransform,TimelineAlpha); 
	const FTransform b = UKismetMathLibrary::TLerp(toTransform,fromTransform,TimelineAlpha); 
	
	const FTransform newTransform = UKismetMathLibrary::SelectTransform(a,b,!property.GetValue().bReverse);
	
	mesh->SetRelativeTransform(newTransform);
}

void UOlimMovableInteractComponent::Interact(FString str, EOlimActorType type)
{
	
}

void UOlimMovableInteractComponent::InteractBP_Implementation(const FString& str, EOlimActorType type)
{
	switch (type)
	{
	case EOlimActorType::EAS_Actor:
		GetProperty(str);
		break;
	case EOlimActorType::EAS_Marker:
		break;
	case EOlimActorType::EAS_Manger:
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

