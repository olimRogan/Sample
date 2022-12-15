// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Engine/DataTable.h"
#include "DefineStructure.generated.h"

USTRUCT(BlueprintType)
struct FLocalizingTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localizing")
		FString Ko;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localizing")
		FString En;
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDefineStructure : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OLIMPLUGIN_API IDefineStructure
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
