// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OlimSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class OLIMPLUGIN_API UOlimSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UOlimSaveGame();
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save Game")
	FString SlotName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save Game")
	int32 SlotIndex;

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Game")
	bool bTouchInterface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Game")
	bool bAndroid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Game")
	bool bLowPerformance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Game")
	int ResolutionNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Game")
	float MouseVal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Game")
	float VolumeVal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Game")
	float CharacterHeight;
	*/
	
};
