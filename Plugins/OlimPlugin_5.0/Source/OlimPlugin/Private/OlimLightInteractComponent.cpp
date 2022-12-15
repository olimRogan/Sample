// Fill out your copyright notice in the Description page of Project Settings.


#include "OlimLightInteractComponent.h"

#include "OlimInteractActor.h"
#include "Components/LightComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UOlimLightInteractComponent::UOlimLightInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOlimLightInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UOlimLightInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InteractTimeline.TickTimeline(DeltaTime);

	if(ComponentState == EOlimLightComponentState::EOLCS_On)
	{
		if(InteractActor)
		{
			ULightComponent* LightComponent = Cast<ULightComponent>(InteractActor.Get()->GetComponentByClass(ULightComponent::StaticClass()));
			if(LightComponent)
			{
				Interaction(CurrentProperty,LightComponent);	
			}
		}
	}
}

void UOlimLightInteractComponent::GetProperty(const FString& name)
{
	if(ComponentState == EOlimLightComponentState::EOLCS_Off)
	{
		// Key : FString, Value : FMovableProperty
		for (TTuple<FString, FOlimLightProperty>& Item : LightList)
		{
			if(name.Equals(Item.Key))
			{
				CurrentProperty = Item.Value;
				PlayTimeline(CurrentProperty.GetValue().Curve);
			}
		}
	}
}

void UOlimLightInteractComponent::Interaction(TOptional<FOlimLightProperty>& property, TObjectPtr<ULightComponent> light)
{
	if(light)
	{
		// 밝기 조절
		{
			const float fromIntensity = property.GetValue().FromIntensity;
			const float toIntensity = property.GetValue().ToIntensity;

			const float lerpIntensity = UKismetMathLibrary::Lerp(fromIntensity,toIntensity,TimelineAlpha);
			const float reverseLerpIntensity = UKismetMathLibrary::Lerp(toIntensity,fromIntensity,TimelineAlpha);

			const float newIntensity = UKismetMathLibrary::SelectFloat(lerpIntensity,reverseLerpIntensity,!property.GetValue().bReverse);

			light->SetIntensity(newIntensity);
		}
		
		// 색상 조절
		{
			const FLinearColor fromColor = property.GetValue().FromColor;
			const FLinearColor toColor = property.GetValue().ToColor;

			const FLinearColor lerpColor = UKismetMathLibrary::LinearColorLerp(fromColor,toColor,TimelineAlpha);
			const FLinearColor reverseLerpColor = UKismetMathLibrary::LinearColorLerp(toColor,fromColor,TimelineAlpha);

			const FLinearColor newLightColor = UKismetMathLibrary::SelectColor(lerpColor,reverseLerpColor,!property.GetValue().bReverse);

			light->SetLightColor(newLightColor);
		}
	}
}

void UOlimLightInteractComponent::Interact(FString str, EOlimActorType type)
{
}

void UOlimLightInteractComponent::InteractBP_Implementation(const FString& str, EOlimActorType type)
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


void UOlimLightInteractComponent::PlayTimeline(UCurveFloat* curve)
{
	if(curve != nullptr)
	{
		FOnTimelineFloat TimelineFloat;
		FOnTimelineEvent FinishEvent;
		TimelineFloat.BindUFunction(this,FName("TimelineProgress"));
		FinishEvent.BindUFunction(this,FName("TimelineFinish"));

		InteractTimeline.AddInterpFloat(curve,TimelineFloat);
		InteractTimeline.SetTimelineFinishedFunc(FinishEvent);

		ComponentState = EOlimLightComponentState::EOLCS_On;
		InteractTimeline.PlayFromStart();
		
	}
}

void UOlimLightInteractComponent::TimelineProgress(float Value)
{
	TimelineAlpha = Value;
}

void UOlimLightInteractComponent::TimelineFinish()
{
	ComponentState = EOlimLightComponentState::EOLCS_Off;
}

