// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

// This class does not need to be modified.

// Actor Type
UENUM(BlueprintType)
enum class EActorType : uint8
{
	EAS_Actor UMETA(DisplayName = "Actor"),
	EAS_Marker UMETA(DisplayName = "Marker"),
	EAS_Manger UMETA(DisplayName = "Manger"),
	
	EAS_DefaultMAX UMETA(DisplayName = "MAX")
};

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
	// CPP
	virtual void Interact(FString string, EActorType type) = 0;
	
	// BP
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void InteractBP(const FString& str,EActorType type);
};
