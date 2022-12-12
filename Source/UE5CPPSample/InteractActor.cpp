// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractActor.h"

#include "LightInteractComponent.h"
#include "MovableInteractComponent.h"
#include "Components/BillboardComponent.h"

// Sets default values
AInteractActor::AInteractActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovableInteractComponent = CreateDefaultSubobject<UMovableInteractComponent>(TEXT("MovableComponent"));
	LightInteractComponent = CreateDefaultSubobject<ULightInteractComponent>(TEXT("LightComponent"));

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootScene);

	BillboardComponent =CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));
	BillboardComponent->SetupAttachment(GetRootComponent());
	BillboardComponent->SetRelativeScale3D(FVector(0.2f));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());

// Add section
#if WITH_EDITOR
#define LOCTEXT_NAMESPACE "Custom Detail"
	static const FName PropertyEditor("PropertyEditor");
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);

	const TSharedRef<FPropertySection> Section = PropertyModule.FindOrCreateSection("Actor", "Product", LOCTEXT("Product", "Product"));
	// 섹션에서 여러 카테고리 추가 가능
	Section->AddCategory("Product");
#undef LOCTEXT_NAMESPACE
#endif
}

// Called when the game starts or when spawned
void AInteractActor::BeginPlay()
{
	Super::BeginPlay();

	if(MovableInteractComponent && LightInteractComponent)
	{
		MovableInteractComponent->InteractActor = this;
		LightInteractComponent->InteractActor = this;
	}
}

// Called every frame
void AInteractActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}