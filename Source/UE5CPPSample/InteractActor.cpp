// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractActor.h"

#include "MovableInteractComponent.h"

// Sets default values
AInteractActor::AInteractActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovableComponent = CreateDefaultSubobject<UMovableInteractComponent>(TEXT("MovableComp"));

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootScene);

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

	if(MovableComponent)
	{
		MovableComponent->InteractActor = this;
	}
}

// Called every frame
void AInteractActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}