// Fill out your copyright notice in the Description page of Project Settings.


#include "ViewPointManager.h"

// Sets default values
AViewPointManager::AViewPointManager()
{
	PrimaryActorTick.bCanEverTick = false;

#if WITH_EDITOR
#define LOCTEXT_NAMESPACE "Custom Detail"
	static const FName PropertyEditor("PropertyEditor");
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);

	// "Actor"는 클래스 타입에 맞게 변경 (예: Actor, Pawn, CharacterMovementComponent)
	// "MySection"은 원하는 섹션 이름으로 변경
	TSharedRef<FPropertySection> Section = PropertyModule.FindOrCreateSection("Actor", "Dev", LOCTEXT("Dev", "Dev"));

	// 섹션에서 여러 카테고리 추가 가능
	Section->AddCategory("Container");
#undef LOCTEXT_NAMESPACE
#endif
}

// Called when the game starts or when spawned
void AViewPointManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AViewPointManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

const TArray<FViewPoint>& AViewPointManager::GetViewPoints() const
{
	return ViewPoints;
}

int AViewPointManager::GetViewPointsNum() const
{
	return ViewPoints.Num();
}

