// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/UMG/Public/UMG.h"
#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "UObject/ConstructorHelpers.h"

#include "LocalizedTextBlock.generated.h"

/**
 * 
 */
UCLASS()
class OLIMPLUGIN_API ULocalizedTextBlock : public UTextBlock
{
	GENERATED_BODY()

public:
	ULocalizedTextBlock();
	virtual void SynchronizeProperties() override;

#if WITH_EDITOR
	virtual void OnCreationFromPalette() override;
	virtual const FText GetPaletteCategory() override;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
	FString KeyString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
	bool NoTransfer;

	UFUNCTION(BlueprintCallable, Category = "Content")
	bool Transfer(const FString& Key, bool dontTransfer);

	UFUNCTION(BlueprintCallable, Category = "Content")
	bool Refresh();

private:
	class UDataTable* LocalizingDataTable;
};
