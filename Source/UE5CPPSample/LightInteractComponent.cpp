// Fill out your copyright notice in the Description page of Project Settings.


#include "LightInteractComponent.h"

#include "InteractActor.h"
#include "Components/LightComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
ULightInteractComponent::ULightInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void ULightInteractComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void ULightInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InteractTimeline.TickTimeline(DeltaTime);

	if(State == ELightState::ELS_On)
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

void ULightInteractComponent::GetProperty(const FString& name)
{
	if(bCanTurnOn)
	{
		// Key : FString, Value : FMovableProperty
		for (TTuple<FString, FLightProperty>& Item : LightList)
		{
			if(name.Equals(Item.Key))
			{
				CurrentProperty = Item.Value;
				PlayTimeline(CurrentProperty.Curve);
			}
		}
	}
}

void ULightInteractComponent::Interaction(TOptional<FLightProperty> property, TObjectPtr<ULightComponent> light)
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

			const FLinearColor NewLightColor = UKismetMathLibrary::SelectColor(lerpColor,reverseLerpColor,!property.GetValue().bReverse);

			light->SetLightColor(NewLightColor);
		}
	}
}

void ULightInteractComponent::Interact(FString string, EActorType type)
{
}

void ULightInteractComponent::InteractBP_Implementation(const FString& str, EActorType type)
{
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

void ULightInteractComponent::PlayTimeline(UCurveFloat* curve)
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
		bCanTurnOn = false;
		State = ELightState::ELS_On;
	}
}

void ULightInteractComponent::TimelineProgress(float Value)
{
	TimelineAlpha = Value;
}

void ULightInteractComponent::TimelineFinish()
{
	bCanTurnOn = true;
	State = ELightState::ELS_Off;
}

