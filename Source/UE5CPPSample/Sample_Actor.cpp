// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample_Actor.h"

// Sets default values
ASample_Actor::ASample_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
void ASample_Actor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASample_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

