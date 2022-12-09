// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableInteractComponent.h"

#include "InteractActor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UMovableInteractComponent::UMovableInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UMovableInteractComponent::BeginPlay()
{
	Super::BeginPlay();
}
// Called every frame
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
		UE_LOG(LogTemp,Warning,TEXT("GetProeprty Num  - %d"),List.Num());
		for (TTuple<FString, FMovableProperty>& Result : List)
		{
			UE_LOG(LogTemp,Warning,TEXT("ResultKey - %s"),*Result.Key);
			if(name.Equals(Result.Key))
			{
				CurrentProperty = Result.Value;
				PlayTimeline(CurrentProperty.Curve);
			}
		}
	}
}

void UMovableInteractComponent::Interaction(TOptional<FMovableProperty> property,TObjectPtr<UStaticMeshComponent> mesh)
{
	if(!property.IsSet() || mesh == nullptr) return;

	const FTransform toTransform = property.GetValue().ToTransform;
	const FTransform fromTransform = property.GetValue().FromTransform;

	const FTransform a = UKismetMathLibrary::TLerp(toTransform,fromTransform,TimelineAlpha); 
	const FTransform b = UKismetMathLibrary::TLerp(fromTransform,toTransform,TimelineAlpha); 
	
	const FTransform newTransform = UKismetMathLibrary::SelectTransform(a,b,property.GetValue().bReverse);
	
	mesh->SetRelativeTransform(newTransform);
}

void UMovableInteractComponent::Interact(FString string)
{
	UE_LOG(LogTemp,Warning,TEXT("%s"),*string);
}

void UMovableInteractComponent::InteractBP_Implementation(const FString& str)
{
	GetProperty(str);
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
	UE_LOG(LogTemp,Warning,TEXT("Alpha - %f"),TimelineAlpha);
}

void UMovableInteractComponent::TimelineFinish()
{
	bCanInteraction = true;
	State = EMovableState::ES_Ready;
}

