// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractActor.generated.h"

UCLASS()
class UE5CPPSAMPLE_API AInteractActor : public AActor
{
	GENERATED_BODY()

public:
	AInteractActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Root Scene
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<USceneComponent> RootScene;

	// Billboard 
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<UBillboardComponent> BillboardComponent;

	// Mesh Component
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	
private:
	// Interact Component - Movable
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, meta = (AllowPrivateAccess ="true"))
	TObjectPtr<class UMovableInteractComponent> MovableComponent;

	// Interact Component - Light
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, meta = (AllowPrivateAccess ="true"))
	TObjectPtr<class ULightInteractComponent> LightInteractComponent;
};
