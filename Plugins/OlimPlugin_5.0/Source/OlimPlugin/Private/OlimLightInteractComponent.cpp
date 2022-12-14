// Fill out your copyright notice in the Description page of Project Settings.
#include "OlimLightInteractComponent.h"
#include "OlimInteractActor.h"
#include "OlimLightActor.h"
#include "Components/LightComponent.h"
#include "Kismet/KismetMathLibrary.h"

UOlimLightInteractComponent::UOlimLightInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UOlimLightInteractComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UOlimLightInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InteractTimeline.TickTimeline(DeltaTime);

	if(ComponentState == EOlimLightComponentState::EOLCS_On)
	{
		if(InteractActor)
		{
			// LightActor is valid
			if(LightActor)
			{
				ULightComponent* Light = Cast<ULightComponent>(LightActor->GetComponentByClass(ULightComponent::StaticClass()));
				if(Light)
				{
					Interaction(CurrentProperty,Light);
				}
			}
			// LightComponent is valid
			if(LightComponent)
			{
				Interaction(CurrentProperty,LightComponent);					
			}
		}
	}
}

void UOlimLightInteractComponent::GetProperty(const FString& name)
{
	if(InteractActor)
	{
		LightComponent = Cast<ULightComponent>(InteractActor.Get()->GetComponentByClass(ULightComponent::StaticClass()));
		if(LightComponent && LightActor == nullptr)
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
		else
		{
			// LightActor Interface Message 전송 -> bCanTurnOn 체크
			if(LightActor->GetClass()->ImplementsInterface(UOlimInteractInterface::StaticClass()))
			{
				IOlimInteractInterface* Interface = Cast<IOlimInteractInterface>(LightActor);
				if(Interface) {Interface->Interact("CPP",EOlimActorType::EAS_Actor);}
			}

			// 불을 켤 수 있는지 검사
			if(LightActor->bCanTurnOn)
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
		}
	}
}

void UOlimLightInteractComponent::Interaction(const TOptional<FOlimLightProperty>& property, ULightComponent* lightComp)
{
	// Light Component O , LightActor X
	if(lightComp)
	{
		// 밝기 조절
		{
			const float fromIntensity = property.GetValue().FromIntensity;
			const float toIntensity = property.GetValue().ToIntensity;

			const float lerpIntensity = UKismetMathLibrary::Lerp(fromIntensity,toIntensity,TimelineAlpha);
			const float reverseLerpIntensity = UKismetMathLibrary::Lerp(toIntensity,fromIntensity,TimelineAlpha);

			const float newIntensity = UKismetMathLibrary::SelectFloat(lerpIntensity,reverseLerpIntensity,!property.GetValue().bReverse);

			lightComp->SetIntensity(newIntensity);
		}
		// 색상 조절
		{
			const FLinearColor fromColor = property.GetValue().FromColor;
			const FLinearColor toColor = property.GetValue().ToColor;

			const FLinearColor lerpColor = UKismetMathLibrary::LinearColorLerp(fromColor,toColor,TimelineAlpha);
			const FLinearColor reverseLerpColor = UKismetMathLibrary::LinearColorLerp(toColor,fromColor,TimelineAlpha);

			const FLinearColor newLightColor = UKismetMathLibrary::SelectColor(lerpColor,reverseLerpColor,!property.GetValue().bReverse);

			lightComp->SetLightColor(newLightColor);
		}
	}
	/*else
	{
		// 밝기 조절
		{
			const float fromIntensity = property.GetValue().FromIntensity;
			const float toIntensity = property.GetValue().ToIntensity;

			const float lerpIntensity = UKismetMathLibrary::Lerp(fromIntensity,toIntensity,TimelineAlpha);
			const float reverseLerpIntensity = UKismetMathLibrary::Lerp(toIntensity,fromIntensity,TimelineAlpha);

			const float newIntensity = UKismetMathLibrary::SelectFloat(lerpIntensity,reverseLerpIntensity,!property.GetValue().bReverse);
			
			ULightComponent* Light = Cast<ULightComponent>(LightActor->GetComponentByClass(ULightComponent::StaticClass()));
			if(Light)
			{
				Light->SetIntensity(newIntensity);
			}
		}
		// 색상 조절
		{
			const FLinearColor fromColor = property.GetValue().FromColor;
			const FLinearColor toColor = property.GetValue().ToColor;

			const FLinearColor lerpColor = UKismetMathLibrary::LinearColorLerp(fromColor,toColor,TimelineAlpha);
			const FLinearColor reverseLerpColor = UKismetMathLibrary::LinearColorLerp(toColor,fromColor,TimelineAlpha);

			const FLinearColor newLightColor = UKismetMathLibrary::SelectColor(lerpColor,reverseLerpColor,!property.GetValue().bReverse);
			
			ULightComponent* Light = Cast<ULightComponent>(LightActor->GetComponentByClass(ULightComponent::StaticClass()));
			if(Light)
			{
				Light->SetLightColor(newLightColor);
			}
		}
	}*/
}

void UOlimLightInteractComponent::Interact(const FString& string, EOlimActorType type)
{
	// TODO CPP
}

void UOlimLightInteractComponent::InteractBP_Implementation(const FString& str,const EOlimActorType type)
{
	switch (type)
	{
	case EOlimActorType::EAS_Actor:
		if(ComponentState == EOlimLightComponentState::EOLCS_Off)
		{
			if(str.Equals("On")) {LightState = EOlimLightState::EOLS_On;}
			else if(str.Equals("Off")) {LightState = EOlimLightState::EOLS_Off;}			
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
	// Curve is nullptr ( message and debug sphere )
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Orange,"Light Curve is nullptr");
		DrawDebugSphere(GetWorld(),GetOwner()->GetActorLocation(),30.f,6,FColor::White,false,5.f,0,1.f);
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

