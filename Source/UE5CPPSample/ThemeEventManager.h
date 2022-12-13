// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractActor.h"
#include "ThemeEventManager.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FDataThemeEvent
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	AActor* InteractableMarker;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName ExecutableEventName;

};

USTRUCT(Atomic, BlueprintType)
struct FThemeEvent
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<FDataThemeEvent> DataThemeEvents;

};

UCLASS()
class UE5CPPSAMPLE_API AThemeEventManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThemeEventManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const TMap<FName, FThemeEvent>* GetThemeEvents() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:



public:
	
protected:

private:
	//need SetUp Data in OutLiner details.
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category ="Container")
	TMap<FName, FThemeEvent> ThemeEvents;

		
		

};
