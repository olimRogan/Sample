// Fill out your copyright notice in the Description page of Project Settings.


#include "OlimSplineActor.h"
#include "Components/BillboardComponent.h"
#include "Components/SplineComponent.h"

AOlimSplineActor::AOlimSplineActor()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SetRootComponent(SplineComponent);
	
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	BillboardComponent->SetupAttachment(SplineComponent);
}

// Called when the game starts or when spawned
void AOlimSplineActor::BeginPlay()
{
	Super::BeginPlay();
}