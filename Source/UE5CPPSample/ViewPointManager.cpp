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

	// "Actor"�� Ŭ���� Ÿ�Կ� �°� ���� (��: Actor, Pawn, CharacterMovementComponent)
	// "MySection"�� ���ϴ� ���� �̸����� ����
	TSharedRef<FPropertySection> Section = PropertyModule.FindOrCreateSection("Actor", "Dev", LOCTEXT("Dev", "Dev"));

	// ���ǿ��� ���� ī�װ� �߰� ����
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

