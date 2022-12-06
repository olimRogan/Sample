// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "OlimEditorSubsystem.generated.h"

/**
 *
 */
UCLASS()
class UE5CPPSAMPLE_API UOlimEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static void FocusActor(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	static void FocusComponent(UMeshComponent* Mesh);
};
