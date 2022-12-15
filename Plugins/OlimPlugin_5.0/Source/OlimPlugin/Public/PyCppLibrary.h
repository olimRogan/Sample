// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

#include "PyCppLibrary.generated.h"

/**
 * 
 */
UCLASS()
class OLIMPLUGIN_API UPyCppLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	UFUNCTION(BlueprintCallable, Category = "Unreal Python")
	static void InsertTargetPoint();
#endif
};
