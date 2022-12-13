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

	// "Actor"�� Ŭ���� Ÿ�Կ� �°� ���� (��: Actor, Pawn, CharacterMovementComponent)
	// "MySection"�� ���ϴ� ���� �̸����� ����
	TSharedRef<FPropertySection> Section = PropertyModule.FindOrCreateSection("Actor", "Dev", LOCTEXT("Dev", "Dev"));

	// ���ǿ��� ���� ī�װ� �߰� ����
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


