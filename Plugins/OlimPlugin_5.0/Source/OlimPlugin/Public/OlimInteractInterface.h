// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OlimInteractInterface.generated.h"

// Actor Type
UENUM(BlueprintType)
enum class EOlimActorType : uint8
{
	EAS_Actor UMETA(DisplayName = "Actor"),
	EAS_Marker UMETA(DisplayName = "Marker"),
	EAS_Manger UMETA(DisplayName = "Manger"),
	
	EAS_DefaultMAX UMETA(DisplayName = "MAX")
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOlimInteractInterface : public UInterface
{
	GENERATED_BODY()
};

class OLIMPLUGIN_API IOlimInteractInterface
{
	GENERATED_BODY()

public:
	// CPP
	virtual void Interact(FString string, EOlimActorType type) = 0;
	
	// BP
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Interact")
	void InteractBP(const FString& str,EOlimActorType type);
};
