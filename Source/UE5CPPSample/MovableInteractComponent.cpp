// Fill out your copyright notice in the Description page of Project Settings.

#include "MovableInteractComponent.h"
#include "InteractActor.h"
#include "Kismet/KismetMathLibrary.h"

UMovableInteractComponent::UMovableInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMovableInteractComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMovableInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InteractTimeline.TickTimeline(DeltaTime);

	if(State == EMovableState::ES_Interacting)
	{
		if(InteractActor)
		{
			Interaction(CurrentProperty,InteractActor.Get()->MeshComponent);	
		}
	}
}

void UMovableInteractComponent::GetProperty(const FString& name)
{
	if(bCanInteraction)
	{
		// Key : FString, Value : FMovableProperty
		for (TTuple<FString, FMovableProperty>& Item : MovableList)
		{
			if(name.Equals(Item.Key))
			{
				CurrentProperty = Item.Value;
				PlayTimeline(CurrentProperty.GetValue().Curve);
			}
		}
	}
}

void UMovableInteractComponent::Interaction(TOptional<FMovableProperty>& property,TObjectPtr<UStaticMeshComponent> mesh)
{
	if(!property.IsSet() || mesh == nullptr) return;

	const FTransform toTransform = property.GetValue().ToTransform;
	const FTransform fromTransform = property.GetValue().FromTransform;

	const FTransform a = UKismetMathLibrary::TLerp(fromTransform,toTransform,TimelineAlpha); 
	const FTransform b = UKismetMathLibrary::TLerp(toTransform,fromTransform,TimelineAlpha); 
	
	const FTransform newTransform = UKismetMathLibrary::SelectTransform(a,b,!property.GetValue().bReverse);
	
	mesh->SetRelativeTransform(newTransform);
}

void UMovableInteractComponent::Interact(FString string, EActorType type)
{
	// TODO CPP
}

void UMovableInteractComponent::InteractBP_Implementation(const FString& str, EActorType type)
{
	//UE_LOG(LogTemp,Warning,TEXT("new Interface - %s -> %s"),*this->GetOwner()->GetName(),*str);
	switch (type)
	{
	case EActorType::EAS_Actor:
		GetProperty(str);
		break;
	case EActorType::EAS_Marker:
		break;
	case EActorType::EAS_Manger:
		break;
	case EActorType::EAS_DefaultMAX:
		break;
	default: ;
	}
}

void UMovableInteractComponent::PlayTimeline(UCurveFloat* curve)
{
	if(curve != nullptr)
	{
		FOnTimelineFloat TimelineFloat;
		FOnTimelineEvent FinishEvent;
		TimelineFloat.BindUFunction(this,FName("TimelineProgress"));
		FinishEvent.BindUFunction(this,FName("TimelineFinish"));

		InteractTimeline.AddInterpFloat(curve,TimelineFloat);
		InteractTimeline.SetTimelineFinishedFunc(FinishEvent);
		
		InteractTimeline.PlayFromStart();
		bCanInteraction = false;
		State = EMovableState::ES_Interacting;
	}
}

void UMovableInteractComponent::TimelineProgress(float Value)
{
	TimelineAlpha = Value;
}

void UMovableInteractComponent::TimelineFinish()
{
	bCanInteraction = true;
	State = EMovableState::ES_Ready;
}

