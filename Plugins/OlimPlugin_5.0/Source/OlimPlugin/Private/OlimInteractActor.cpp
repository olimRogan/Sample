// Fill out your copyright notice in the Description page of Project Settings.


#include "OlimInteractActor.h"

#include "OlimLightInteractComponent.h"
#include "OlimMovableInteractComponent.h"
#include "Components/BillboardComponent.h"

// Sets default values
AOlimInteractActor::AOlimInteractActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	MovableInteractComponent = CreateDefaultSubobject<UOlimMovableInteractComponent>(TEXT("MovableComponent"));
	LightInteractComponent = CreateDefaultSubobject<UOlimLightInteractComponent>(TEXT("LightComponent"));
	
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootScene);

	BillboardComponent =CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));
	BillboardComponent->SetupAttachment(GetRootComponent());
	BillboardComponent->SetRelativeScale3D(FVector(0.2f));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AOlimInteractActor::BeginPlay()
{
	Super::BeginPlay();

	if(MovableInteractComponent.Get()) {MovableInteractComponent.Get()->InteractActor = this;}
	if(LightInteractComponent.Get()) {LightInteractComponent.Get()->InteractActor = this;}
	
}

// Called every frame
void AOlimInteractActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

