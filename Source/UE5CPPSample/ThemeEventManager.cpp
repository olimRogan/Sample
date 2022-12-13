// Fill out your copyright notice in the Description page of Project Settings.


#include "ThemeEventManager.h"

// Sets default values
AThemeEventManager::AThemeEventManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
void AThemeEventManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThemeEventManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

const TMap<FName, FThemeEvent>* AThemeEventManager::GetThemeEvents() const
{
	return &ThemeEvents;
}


