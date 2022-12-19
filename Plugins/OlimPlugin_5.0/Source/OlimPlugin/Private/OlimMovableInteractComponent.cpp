// Fill out your copyright notice in the Description page of Project Settings.

#include "OlimMovableInteractComponent.h"
#include "OlimInteractActor.h"
#include "Selection.h"
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
		if(MovableList.IsEmpty())
		{
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Orange,
					FString::Printf(TEXT("%s - Movable List 가 비어 있습니다."),*this->GetOwner()->GetName()));
			DrawDebugSphere(GetWorld(),GetOwner()->GetActorLocation(),15.f,6,FColor::Orange,false,5.f,0,1.f);
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
	// TODO CPP
}

void UOlimMovableInteractComponent::InteractBP_Implementation(const FString& str, EOlimActorType type)
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

