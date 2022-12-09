// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE5CPPSAMPLE_API IInteractInterface
{
	GENERATED_BODY()

public:
	virtual void Interact(FString string) = 0;

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void InteractBP(const FString& str);
};
