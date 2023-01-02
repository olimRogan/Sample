
// Fill out your copyright notice in the Description page of Project Settings.


#include "OlimSplineInteractComponent.h"

#include "OlimInteractActor.h"
#include "OlimSplineActor.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UOlimSplineInteractComponent::UOlimSplineInteractComponent() :
	Speed(100.f),
	TurnSpeed(3.f)
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOlimSplineInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	OlimInteractActor = Cast<AOlimInteractActor>(GetOwner());
	if(OlimInteractActor && SplineActor)
	{
		InitRotation = OlimInteractActor->GetActorRotation();
		const FVector loc = SplineActor->GetSpline()->GetLocationAtTime(0.f,ESplineCoordinateSpace::World);
		OlimInteractActor->SetActorLocation(loc);
	}
} 

// Called every frame
void UOlimSplineInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(OlimInteractActor && SplineActor)
	{
		const float MaxLength = SplineActor->GetSpline()->GetSplineLength();

		switch (State)
		{
		case EOlimSplineComponentState::EOSCS_On:
			if(Time >= 1.f)
			{
				Time = 0.f;
			}
			else
			{
				const float SplineLength = SplineActor->GetSpline()->GetSplineLength();
				Time+=(DeltaTime*(Speed/SplineLength));
				const FVector loc = SplineActor->GetSpline()->GetLocationAtTime(Time,ESplineCoordinateSpace::World,true);
				const FRotator rot = SplineActor->GetSpline()->GetRotationAtTime(Time,ESplineCoordinateSpace::World,true);
				const FRotator interpRot = UKismetMathLibrary::RInterpTo(OlimInteractActor->GetActorRotation(),rot,DeltaTime,TurnSpeed);
				OlimInteractActor->SetActorLocation(loc);
				OlimInteractActor->SetActorRotation(interpRot);
				
				CurrentLength+=(DeltaTime*Speed);
				if(CurrentLength >= MaxLength) { State = EOlimSplineComponentState::EOSCS_Off;}
			}
			break;
		case EOlimSplineComponentState::EOSCS_Off:
			if(Time <= 0.f)
			{
				Time = 0.f;
				const FRotator interpRot = UKismetMathLibrary::RInterpTo(OlimInteractActor->GetActorRotation(),InitRotation,DeltaTime,TurnSpeed);
				OlimInteractActor->SetActorRotation(interpRot);
			}
			else
			{
				const float SplineLength = SplineActor->GetSpline()->GetSplineLength();
				Time-=(DeltaTime*(Speed/SplineLength));
				const FVector loc = SplineActor->GetSpline()->GetLocationAtTime(Time,ESplineCoordinateSpace::World,true);
				const FRotator rot = SplineActor->GetSpline()->GetRotationAtTime(Time,ESplineCoordinateSpace::World,true);
				const FRotator newRot = UKismetMathLibrary::FindLookAtRotation(OlimInteractActor->GetActorLocation(),
					(UKismetMathLibrary::GetForwardVector(rot)*-50.f)+loc);
				const FRotator interpRot = UKismetMathLibrary::RInterpTo(OlimInteractActor->GetActorRotation(),newRot,DeltaTime,TurnSpeed);
				OlimInteractActor->SetActorLocation(loc);
				OlimInteractActor->SetActorRotation(interpRot);
				CurrentLength-=(DeltaTime*Speed);
			}
			break;
		case EOlimSplineComponentState::EOSCS_DefaultMAX:
			break;
		default: ;
		}
	}
}

void UOlimSplineInteractComponent::Interact(const FString& string, EOlimActorType type)
{
	// TODO CPP
}

void UOlimSplineInteractComponent::InteractBP_Implementation(const FString& str, const EOlimActorType type)
{
	if(str.Equals("On") ) {State = EOlimSplineComponentState::EOSCS_On;}
	if(str.Equals("Off") ) {State = EOlimSplineComponentState::EOSCS_Off;}
}

