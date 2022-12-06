// Fill out your copyright notice in the Description page of Project Settings.


#include "OlimEditorSubsystem.h"

#include "EngineUtils.h"
#include "LevelEditorViewport.h"
#include "Selection.h"

void UOlimEditorSubsystem::FocusActor(AActor* Actor)
{
	UWorld* World = GEditor->GetEditorWorldContext().World()->GetWorld();
	if (World)
	{
		if (!Actor->IsSelectedInEditor())

		{
			GEditor->GetSelectedActors()->DeselectAll();
			GEditor->SelectActor(Actor, true, false);
			GEditor->Exec(World, TEXT("Camera Align"));
		}
	}
}

void UOlimEditorSubsystem::FocusComponent(UMeshComponent* Mesh)
{
	UWorld* World = GEditor->GetEditorWorldContext().World()->GetWorld();
	if (World)
	{
		if (!Mesh->IsSelectedInEditor())

		{
			GEditor->GetSelectedComponents()->DeselectAll();
			GEditor->SelectComponent(Mesh, true, false);
			GEditor->Exec(World, TEXT("Camera Align"));
		}
	}
}
