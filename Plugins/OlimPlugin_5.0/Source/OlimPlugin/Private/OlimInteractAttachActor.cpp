// Fill out your copyright notice in the Description page of Project Settings.


#include "OlimInteractAttachActor.h"

#include "OlimInteractActor.h"
#include "OlimLightInteractComponent.h"
#include "OlimMovableInteractComponent.h"
#include "Components/BillboardComponent.h"

AOlimInteractAttachActor::AOlimInteractAttachActor()
{
	PrimaryActorTick.bCanEverTick = true;

	MovableInteractComponent = CreateDefaultSubobject<UOlimMovableInteractComponent>(TEXT("MovableComponent"));
	LightInteractComponent = CreateDefaultSubobject<UOlimLightInteractComponent>(TEXT("LightComponent"));
	
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootScene);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());

	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	BillboardComponent->SetupAttachment(GetRootComponent());
}

void AOlimInteractAttachActor::BeginPlay()
{
	Super::BeginPlay();
	
	if(MovableInteractComponent.Get()) {MovableInteractComponent.Get()->MeshComponent = MeshComponent;}
	//if(LightInteractComponent.Get()) {LightInteractComponent.Get()->InteractActor = this;}
}

void AOlimInteractAttachActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if(InteractActor)
	{
		AttachToComponent(InteractActor->AttachPoint,FAttachmentTransformRules::KeepWorldTransform);
		SetActorRelativeTransform(FTransform());
	}
	else
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void AOlimInteractAttachActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(InteractActor)
	{
		AttachToComponent(InteractActor->AttachPoint,FAttachmentTransformRules::KeepRelativeTransform);
	}
}
