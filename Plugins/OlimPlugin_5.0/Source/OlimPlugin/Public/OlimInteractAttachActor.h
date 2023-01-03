// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OlimInteractAttachActor.generated.h"

UCLASS(HideCategories = ("Activation","Cooking","Collision","AssetUserData","Physics","Tags","Navigation","VirtualTexture"))
class OLIMPLUGIN_API AOlimInteractAttachActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOlimInteractAttachActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaSeconds) override;
	
	// Root Scene
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "Component")
	TObjectPtr<USceneComponent> RootScene = nullptr;

	// Billboard 
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "Component")
	class UBillboardComponent* BillboardComponent = nullptr;
	
	// Mesh Component
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "Product")
	TObjectPtr<UStaticMeshComponent> MeshComponent = nullptr;
	
private:
	// Interact Component - Movable
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess ="true"))
	TObjectPtr<class UOlimMovableInteractComponent> MovableInteractComponent = nullptr;
	
	// Interact Component - Light
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess ="true"))
	TObjectPtr<class UOlimLightInteractComponent> LightInteractComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Product", meta = (AllowPrivateAccess ="true"))
	TObjectPtr<class AOlimInteractActor> InteractActor;
};
