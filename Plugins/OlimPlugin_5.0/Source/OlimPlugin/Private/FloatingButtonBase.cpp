// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/FloatingButtonBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFloatingButtonBase::AFloatingButtonBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; 

	//StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	//SetRootComponent(StaticMeshComp);
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> BasicShapePlane(TEXT("/Engine/BasicShapes/Plane.Plane"));
	//if (BasicShapePlane.Succeeded())
	//{	
	//	static ConstructorHelpers::FObjectFinder<UMaterial> FloatingButtonMaterial(TEXT("/OlimPlugin/COMMONS/Development/05_FloatingIcon/Base/FloatingIconMaterial.FloatingIconMaterial"));
	//	if (FloatingButtonMaterial.Succeeded())
	//	{
	//		BasicShapePlane.Object->SetMaterial(0, FloatingButtonMaterial.Object);
	//	}
	//
	//	StaticMeshComp->SetStaticMesh(BasicShapePlane.Object);
	//	StaticMeshComp->SetWorldScale3D(FVector(0.2f));
	//	StaticMeshComp->SetRelativeRotation(FRotator(0.f, -180.f, 90.f).Quaternion());
	//	UE_LOG(LogTemp, Log, TEXT("BasicShapePlane"));
	//}
}

// Called when the game starts or when spawned
void AFloatingButtonBase::BeginPlay()
{ 
	Super::BeginPlay();  
}

// Called every frame
void AFloatingButtonBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

