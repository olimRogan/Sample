// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OlimInteractInterface.h"
#include "GameFramework/Actor.h"
#include "OlimLightActor.generated.h"

enum class EOlimMovementState : uint8;
enum class EOlimLightState : uint8;
UCLASS()
class OLIMPLUGIN_API AOlimLightActor : public AActor, public IOlimInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOlimLightActor();

	bool bCanTurnOn = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Product")
	TArray<TObjectPtr<class AOlimInteractActor>> InteractActors;
	
	UPROPERTY()
	TArray<TObjectPtr<AOlimInteractActor>> ActiveActors;
	
	// Interface 함수
	virtual void Interact(const FString& string,EOlimActorType type) override;
};
